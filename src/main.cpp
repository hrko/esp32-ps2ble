// clang-format off
#include <fmt/core.h>
#include <boost/circular_buffer.hpp>
#include <Arduino.h>
#include <NimBLEDevice.h>
// clang-format on

#include <cstdio>
#include <map>

#include "hid/keyboard.hpp"
#include "hid/mouse.hpp"
#include "hid/report_map.hpp"
#include "key_translate.hpp"
#include "logging.hpp"
#include "secrets.hpp"
extern "C" {
#include <esp_hid_common.h>
}

#include <ArduinoJson.h>
#include <ArduinoNvs.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

#include <PS2Keyboard.hpp>
#include <PS2Mouse.hpp>

AsyncWebServer server(80);
esp32_ps2dev::PS2Mouse mouse(17, 16);
esp32_ps2dev::PS2Keyboard keyboard(19, 18);

const char CUUID_HID_SERVICE[] = "1812";
const char CUUID_HID_INFORMATION[] = "2A4A";
const char CUUID_HID_REPORT_MAP[] = "2A4B";
const char CUUID_HID_CONTROL_POINT[] = "2A4C";
const char CUUID_HID_REPORT_DATA[] = "2A4D";
const char DUUID_HID_REPORT_REFERENCE[] = "2908";

enum class ScanMode : uint8_t {
  NewDeviceOnly,
  NewDeviceAndBoundedDevice,
  BoundedDeviceOnly,
  Disabled,
  _Count,
};

bool isValidScanMode(uint8_t scanMode) { return scanMode < static_cast<uint8_t>(ScanMode::_Count); }

constexpr auto DEFAULT_SCAN_MODE = ScanMode::BoundedDeviceOnly;

QueueHandle_t xQueueScanMode;
QueueHandle_t xQueueDeviceToConnect;
QueueHandle_t xQueueLastConnectedDevice;

using HandleReportIDMap = std::map<uint16_t, reportID_t>;
std::map<NimBLEAddress, HandleReportIDMap> HandleReportIDMapCache;
std::map<NimBLEAddress, ReportMap*> ReportMapCache;

void subscribeToHIDService(NimBLEClient* client);

std::string stripColon(const std::string& str) {
  auto output = std::string();
  for (auto& c : str) {
    if (c != ':') {
      output += c;
    }
  }
  return output;
}

class ClientCallbacks : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) {
    auto output = fmt::format("Connected to: {}", pClient->getPeerAddress().toString());
    PS2BLE_LOGI(output);
  };

  void onDisconnect(NimBLEClient* pClient) {
    auto output = fmt::format("Disconnected from: {}", pClient->getPeerAddress().toString());
    PS2BLE_LOGI(output);
  };

  bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {
    auto reject = false;
    if (params->itvl_min < 12) {  // 12 * 1.25ms = 15ms
      reject = true;
    }

    if (reject) {
      PS2BLE_LOGI(fmt::format(
          "Rejected connection parameters update request from: {}, itvl_min: {}, itvl_max: {}, latency: {}, supervision_timeout: {}",
          pClient->getPeerAddress().toString(), params->itvl_min, params->itvl_max, params->latency, params->supervision_timeout));
      return false;
    } else {
      PS2BLE_LOGI(fmt::format(
          "Accepted connection parameters update request from: {}, itvl_min: {}, itvl_max: {}, latency: {}, supervision_timeout: {}",
          pClient->getPeerAddress().toString(), params->itvl_min, params->itvl_max, params->latency, params->supervision_timeout));
      return true;
    }
  };

  uint32_t onPassKeyRequest() {
    PS2BLE_LOGI("Client Passkey Request");
    return 123456;
  };

  bool onConfirmPIN(uint32_t pass_key) {
    auto output = fmt::format("The passkey YES/NO number: {}", pass_key);
    PS2BLE_LOGI(output);
    return true;
  };

  void onAuthenticationComplete(ble_gap_conn_desc* desc) {
    PS2BLE_LOGI("Pairing completed");
    if (!desc->sec_state.encrypted) {
      PS2BLE_LOGW("WARNING: Link is not encrypted");
    }
  };
};

static ClientCallbacks clientCB;

bool isBondedDevice(NimBLEAdvertisedDevice* advertisedDevice) {
  auto addr = advertisedDevice->getAddress();
  auto bondedNum = NimBLEDevice::getNumBonds();
  for (size_t i = 0; i < bondedNum; i++) {
    auto bondedAddr = NimBLEDevice::getBondedAddress(i);
    if (addr == bondedAddr) {
      return true;
    }
  }
  return false;
}

bool isAdvertisingHIDService(NimBLEAdvertisedDevice* advertisedDevice) {
  if (advertisedDevice->haveServiceUUID() && advertisedDevice->isAdvertisingService(NimBLEUUID(CUUID_HID_SERVICE))) {
    return true;
  }
  return false;
}

class AdvertisedDeviceCallbacksNewDeviceOnly : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    auto advType = advertisedDevice->getAdvType();
    if (isBondedDevice(advertisedDevice)) return;
    if (isAdvertisingHIDService(advertisedDevice)) {
      xQueueSend(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY);
      PS2BLE_LOGI("Found new device advertising HID service");
    }
  };
};

class AdvertisedDeviceCallbacksNewDeviceAndBoundedDevice : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    if (isBondedDevice(advertisedDevice) || isAdvertisingHIDService(advertisedDevice)) {
      xQueueSend(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY);
      if (isBondedDevice(advertisedDevice)) {
        PS2BLE_LOGI("Found bonded device");
      } else {
        PS2BLE_LOGI("Found new device advertising HID service");
      }
    }
  };
};

class AdvertisedDeviceCallbacksBoundedDeviceOnly : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    if (isBondedDevice(advertisedDevice)) {
      xQueueSend(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY);
      PS2BLE_LOGI("Found bonded device");
    }
  };
};

void scanCompleteCB(NimBLEScanResults) { PS2BLE_LOGI("Scan complete"); }

void taskScan(void* arg) {
  AdvertisedDeviceCallbacksNewDeviceOnly newDeviceOnlyCallbacks;
  AdvertisedDeviceCallbacksNewDeviceAndBoundedDevice newDeviceAndBoundedDeviceCallbacks;
  AdvertisedDeviceCallbacksBoundedDeviceOnly boundedDeviceOnlyCallbacks;
  NimBLEScan* scan = NimBLEDevice::getScan();
  scan->setInterval(45);
  scan->setWindow(15);
  scan->setActiveScan(true);
  ScanMode lastScanMode = DEFAULT_SCAN_MODE;
  ScanMode scanMode;

  while (true) {
    delay(100);
    if (xQueuePeek(xQueueScanMode, &scanMode, portMAX_DELAY) == pdTRUE) {
      if (scanMode == ScanMode::Disabled) {
        PS2BLE_LOGV("Scan disabled");
        continue;
      }

      if (scan->isScanning()) {
        if (scanMode == lastScanMode) {
          PS2BLE_LOGV("Scan mode unchanged");
          continue;
        } else {
          PS2BLE_LOGD("Scan mode changed, stopping scan");
          scan->stop();
        }
      }

      switch (scanMode) {
        case ScanMode::NewDeviceOnly:
          PS2BLE_LOGI("Scan mode: NewDeviceOnly");
          scan->setAdvertisedDeviceCallbacks(&newDeviceOnlyCallbacks);
          scan->setFilterPolicy(BLE_HCI_SCAN_FILT_NO_WL);
          break;
        case ScanMode::NewDeviceAndBoundedDevice:
          PS2BLE_LOGI("Scan mode: NewDeviceAndBoundedDevice");
          scan->setAdvertisedDeviceCallbacks(&newDeviceAndBoundedDeviceCallbacks);
          scan->setFilterPolicy(BLE_HCI_SCAN_FILT_NO_WL);
          break;
        case ScanMode::BoundedDeviceOnly:
          PS2BLE_LOGI("Scan mode: BoundedDeviceOnly");
          scan->setAdvertisedDeviceCallbacks(&boundedDeviceOnlyCallbacks);
          scan->setActiveScan(true);
          for (size_t i = 0; i < NimBLEDevice::getNumBonds(); i++) {
            const auto& addr = NimBLEDevice::getBondedAddress(i);
            auto ok = NimBLEDevice::whiteListAdd(addr);
            if (!ok) {
              PS2BLE_LOGE(fmt::format("Failed to add {} to whitelist", addr.toString()));
            }
          }
          scan->setFilterPolicy(BLE_HCI_SCAN_FILT_USE_WL);
          break;
        default:
          continue;
      }

      auto ok = scan->start(0, scanCompleteCB);
      if (!ok) {
        PS2BLE_LOGV("Failed to start scan");
      } else {
        lastScanMode = scanMode;
        PS2BLE_LOGI("Scan started");
      }
    }
  }
}

const char SERVICE_UUID_GENERIC_ACCESS[] = "1800";
const char CHARACTERISTIC_UUID_DEVICE_NAME[] = "2A00";
const char CHARACTERISTIC_UUID_APPEARANCE[] = "2A01";
const std::uint16_t APPEARANCE_GENERIC_HID = 960;
const std::uint16_t APPEARANCE_HID_KEYBOARD = 961;
const std::uint16_t APPEARANCE_HID_MOUSE = 962;

std::uint16_t getAppearance(NimBLEClient* client) {
  auto service = client->getService(NimBLEUUID(SERVICE_UUID_GENERIC_ACCESS));
  if (service == nullptr) {
    PS2BLE_LOGE("Generic Access service not found");
    return 0;
  }
  auto characteristic = service->getCharacteristic(NimBLEUUID(CHARACTERISTIC_UUID_APPEARANCE));
  if (characteristic == nullptr) {
    PS2BLE_LOGE("Appearance characteristic not found");
    return 0;
  }
  auto value = characteristic->readValue();
  if (value.length() != 2) {
    PS2BLE_LOGE("Appearance value length is not 2");
    return 0;
  }
  auto appearance = value[0] | (value[1] << 8);
  return appearance;
}

std::string getAppearanceName(std::uint16_t appearance) {
  switch (appearance) {
    case APPEARANCE_GENERIC_HID:
      return "Generic HID";
    case APPEARANCE_HID_KEYBOARD:
      return "Keyboard";
    case APPEARANCE_HID_MOUSE:
      return "Mouse";
    default:
      return "Unknown";
  }
}

bool saveAppearanceToNVS(NimBLEClient* client) {
  auto appearance = getAppearance(client);
  if (appearance == 0) {
    PS2BLE_LOGE("Failed to get appearance");
    return false;
  }
  auto addr = client->getPeerAddress();
  auto key = stripColon(addr.toString() + "AP");
  auto ok = NVS.setInt(key.c_str(), appearance);
  if (!ok) {
    PS2BLE_LOGE("Failed to save bonded device appearance to NVS");
    return false;
  }
  PS2BLE_LOGI(fmt::format("Saved bonded device appearance to NVS: {} = {}", addr.toString(), appearance));
  return true;
}

std::uint16_t readAppearanceFromNVS(const NimBLEAddress& addr) {
  auto key = stripColon(addr.toString() + "AP");
  std::uint16_t appearance = NVS.getInt(key.c_str());
  if (appearance == -1) {
    PS2BLE_LOGE("Failed to read appearance from NVS");
    return 0;
  }
  return appearance;
}

std::string getDeviceName(NimBLEClient* client) {
  auto service = client->getService(NimBLEUUID(SERVICE_UUID_GENERIC_ACCESS));
  if (service == nullptr) {
    PS2BLE_LOGE("Generic Access service not found");
    return "";
  }
  auto characteristic = service->getCharacteristic(NimBLEUUID(CHARACTERISTIC_UUID_DEVICE_NAME));
  if (characteristic == nullptr) {
    PS2BLE_LOGE("Device Name characteristic not found");
    return "";
  }
  auto value = characteristic->readValue();
  auto deviceName = std::string(value.c_str());
  return deviceName;
}

bool saveDeviceNameToNVS(NimBLEClient* client) {
  auto name = getDeviceName(client);
  if (name == "") {
    PS2BLE_LOGE("Failed to get device name");
    return false;
  }
  auto addr = client->getPeerAddress();
  auto key = stripColon(addr.toString());
  auto ok = NVS.setString(key.c_str(), name.c_str());
  if (!ok) {
    PS2BLE_LOGE("Failed to save bonded device name to NVS");
    return false;
  }
  PS2BLE_LOGI(fmt::format("Saved bonded device name to NVS: {} = {}", addr.toString(), name));
  return true;
}

std::string readDeviceNameFromNVS(const NimBLEAddress& addr) {
  auto key = stripColon(addr.toString());
  auto name = NVS.getString(key.c_str());
  if (name == "") {
    PS2BLE_LOGE("Failed to read device name from NVS");
    return "";
  }
  return name.c_str();
}

void taskConnect(void* arg) {
  NimBLEAdvertisedDevice* advertisedDevice;
  while (true) {
    if (xQueueReceive(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY) == pdTRUE) {
      // Save current scan mode.
      ScanMode currentScanMode = ScanMode::BoundedDeviceOnly;
      auto ret = xQueuePeek(xQueueScanMode, &currentScanMode, 0);
      if (ret != pdTRUE) {
        PS2BLE_LOGE("xQueuePeek failed for xQueueScanMode");
      }

      // Prevent taskScan to resatart scan.
      auto scanMode = ScanMode::Disabled;
      ret = xQueueOverwrite(xQueueScanMode, &scanMode);
      if (ret != pdTRUE) {
        PS2BLE_LOGE("xQueueOverwrite failed for xQueueScanMode");
      }

      // Try to use the existing client to reduce the connection time.
      NimBLEClient* client = NimBLEDevice::getClientByPeerAddress(advertisedDevice->getAddress());
      // If the existing client is not available, create a new client.
      if (client == nullptr) {
        client = NimBLEDevice::createClient();
      }

      client->setClientCallbacks(&clientCB, false);
      const auto minInterval = 12;          // 12 * 1.25ms = 15ms
      const auto maxInterval = 12;          // 12 * 1.25ms = 15ms
      const auto slaveLatency = 0;          // The number of packets allowed to skip. Helps peripheral save power.
      const auto supervisionTimeout = 510;  // 51 * 10ms = 510ms
      client->setConnectionParams(minInterval, maxInterval, slaveLatency, supervisionTimeout);
      client->setConnectTimeout(5);  // The timeout to wait for connection attempt to complete.
      auto isConnected = client->connect(advertisedDevice);
      if (isConnected) {
        auto addr = client->getPeerAddress();
        ret = xQueueOverwrite(xQueueLastConnectedDevice, &addr);
        if (ret != pdTRUE) {
          PS2BLE_LOGE("xQueueOverwrite failed for xQueueLastConnectedDevice");
        }
        PS2BLE_LOGI(fmt::format("Connected to: {}", client->getPeerAddress().toString()));
        auto ok = saveDeviceNameToNVS(client);
        if (!ok) {
          PS2BLE_LOGE("Failed to save device name to NVS");
        }
        ok = saveAppearanceToNVS(client);
        if (!ok) {
          PS2BLE_LOGE("Failed to save appearance to NVS");
        }
        subscribeToHIDService(client);
      } else {
        PS2BLE_LOGI(fmt::format("Failed to connect to: {}", client->getPeerAddress().toString()));
        BLEDevice::deleteClient(client);
      }

      // Restore scan mode.
      ret = xQueueOverwrite(xQueueScanMode, &currentScanMode);
      if (ret != pdTRUE) {
        PS2BLE_LOGE("xQueueOverwrite failed for xQueueScanMode");
      }
    }
  }
  vTaskDelete(NULL);
}

std::map<std::pair<NimBLEAddress, reportID_t>, KeyboardReport> LastKeyboardReport;

void notifyCallbackKeyboardHIDReport(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  auto addr = pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress();
  auto handle = pRemoteCharacteristic->getHandle();
  auto reportID = HandleReportIDMapCache[addr][handle];
  auto reportMap = ReportMapCache[addr];
  auto reportItemList = reportMap->getInputReportItemList(reportID);
  auto report = decodeKeyboardInputReport(pData, *reportItemList);
  auto pressedKeys = report.getPressedKeys();
  auto usagePage = static_cast<UsagePage>(report.getUsagePage());
  // if last report is not empty, compare with current report
  if (LastKeyboardReport.find({addr, reportID}) != LastKeyboardReport.end()) {
    auto lastReport = LastKeyboardReport[{addr, reportID}];
    auto currentReport = report;
    auto lastPressedKeys = lastReport.getPressedKeys();
    auto currentPressedKeys = pressedKeys;

    // check for key up
    for (auto& lastPressedKey : lastPressedKeys) {
      auto isKeyUp = std::find(currentPressedKeys.begin(), currentPressedKeys.end(), lastPressedKey) == currentPressedKeys.end();
      if (isKeyUp) {
        auto scanCode = getScanCode(lastPressedKey, ScanCodeType::Break, usagePage, ScanCodeSet::Set2);
        if (scanCode == nullptr) {
          PS2BLE_LOGE(fmt::format("scanCode not found for 0x{:04X}", lastPressedKey));
          continue;
        }
        auto scanCodeData = *scanCode->getCode();
        keyboard.send_scancode(scanCodeData);
      }
    }
    // check for key down
    for (auto& currentPressedKey : currentPressedKeys) {
      auto isKeyDown = std::find(lastPressedKeys.begin(), lastPressedKeys.end(), currentPressedKey) == lastPressedKeys.end();
      if (isKeyDown) {
        auto scanCode = getScanCode(currentPressedKey, ScanCodeType::Make, usagePage, ScanCodeSet::Set2);
        if (scanCode == nullptr) {
          PS2BLE_LOGE(fmt::format("scanCode not found for 0x{:04X}", currentPressedKey));
          continue;
        }
        auto scanCodeData = *scanCode->getCode();
        keyboard.send_scancode(scanCodeData);
      }
    }
  } else {
    // if last report is empty, send all key down
    for (auto& pressedKey : pressedKeys) {
      auto scanCode = getScanCode(pressedKey, ScanCodeType::Make, usagePage, ScanCodeSet::Set2);
      if (scanCode == nullptr) {
        PS2BLE_LOGE(fmt::format("scanCode not found for 0x{:04X}", pressedKey));
        continue;
      }
      auto scanCodeData = *scanCode->getCode();
      keyboard.send_scancode(scanCodeData);
    }
  }
  // update last report
  LastKeyboardReport[{addr, reportID}] = report;
  PS2BLE_LOGI(report.toString());
}

class MouseStatus {
 public:
  boost::circular_buffer<MouseReport> hidReportBuf = boost::circular_buffer<MouseReport>(10);
  unsigned long lastPs2ReportTimeMicros = 0;
};
std::map<std::pair<NimBLEAddress, reportID_t>, MouseStatus> MouseStatusMap;
void IRAM_ATTR notifyCallbackMouseHIDReport(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length,
                                            bool isNotify) {
  constexpr auto MinPs2ReportIntervalMicros = 16667UL;

  const auto addr = pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress();
  const auto handle = pRemoteCharacteristic->getHandle();
  const auto reportID = HandleReportIDMapCache[addr][handle];
  const auto reportMap = ReportMapCache[addr];
  const auto reportItemList = reportMap->getInputReportItemList(reportID);
  auto currentHidReport = decodeMouseInputReport(pData, *reportItemList);
  const auto currentTimeMicros = micros();
  PS2BLE_LOGV(currentHidReport.toString());

  // Initialize mouse status if not initialized.
  if (MouseStatusMap.find({addr, reportID}) == MouseStatusMap.end()) {
    MouseStatusMap[{addr, reportID}] = MouseStatus();
  }

  // Load mouse status.
  auto& mouseStatus = MouseStatusMap[{addr, reportID}];
  // const auto lastPs2ReportTimeMicros = mouseStatus.lastPs2ReportTimeMicros;

  bool sendPs2Report = false;

  do {
    // If enough time has passed since last PS/2 report, send PS/2 report.
    if (currentTimeMicros > mouseStatus.lastPs2ReportTimeMicros + MinPs2ReportIntervalMicros) {
      sendPs2Report = true;
      break;
    }

    // If last report is full, send PS/2 report.
    if (mouseStatus.hidReportBuf.full()) {
      sendPs2Report = true;
      break;
    }

    // If HID report buffer is not empty, compare last HID report with current HID report.
    if (!mouseStatus.hidReportBuf.empty()) {
      const auto& lastHidReport = mouseStatus.hidReportBuf.back();
      // Check if button state is changed since last report.
      auto isButtonChanged = false;
      constexpr auto PS2ButtonCount = 5;
      for (size_t i = 0; i < PS2ButtonCount; i++) {
        if (currentHidReport.isButtonPressed[i] != lastHidReport.isButtonPressed[i]) {
          isButtonChanged = true;
          break;
        }
      }
      if (isButtonChanged) {
        sendPs2Report = true;
        break;
      }
      // Check if wheel count is changed since last report.
      if (currentHidReport.wheelVertical != lastHidReport.wheelVertical) {
        sendPs2Report = true;
        break;
      }
    }
  } while (false);

  // If PS/2 report is needed, send PS/2 report.
  if (sendPs2Report) {
    for (auto& hidReport : mouseStatus.hidReportBuf) {
      currentHidReport.x += hidReport.x;
      currentHidReport.y += hidReport.y;
    }
    mouse.send_report(currentHidReport.x, -currentHidReport.y, currentHidReport.wheelVertical, currentHidReport.isButtonPressed[0],
                      currentHidReport.isButtonPressed[1], currentHidReport.isButtonPressed[2], currentHidReport.isButtonPressed[3],
                      currentHidReport.isButtonPressed[4]);
    mouseStatus.hidReportBuf.clear();
  } else {
    // If PS/2 report is not needed, push HID report to buffer.
    mouseStatus.hidReportBuf.push_back(currentHidReport);
  }
}

void cacheReportMap(NimBLEClient* client, NimBLERemoteService* service) {
  auto isReportMapCached = ReportMapCache.find(client->getPeerAddress()) != ReportMapCache.end();
  if (!isReportMapCached) {
    PS2BLE_LOGI("Caching report map");
    auto characteristic = service->getCharacteristic(CUUID_HID_REPORT_MAP);
    if (characteristic != nullptr) {
      auto value = characteristic->readValue();
      auto rawReportMap = value.data();
      auto rawReportMapLength = value.length();
      auto reportMap = new ReportMap(rawReportMap, rawReportMapLength);
      ReportMapCache[client->getPeerAddress()] = reportMap;
    }
    PS2BLE_LOGI("Cached report map");
  } else {
    PS2BLE_LOGI("Report map already cached");
  }
}

void cacheHandleReportIDMap(NimBLEClient* client, const std::vector<NimBLERemoteCharacteristic*>& characteristicsHidReport) {
  auto isHandleReportIDMapCached = HandleReportIDMapCache.find(client->getPeerAddress()) != HandleReportIDMapCache.end();
  if (!isHandleReportIDMapCached) {
    HandleReportIDMap handleReportIDMap;
    for (auto& c : characteristicsHidReport) {
      auto handle = c->getHandle();
      auto desc = c->getDescriptor(NimBLEUUID(DUUID_HID_REPORT_REFERENCE));
      auto value = desc->readValue();
      if (value.size() != 2) continue;
      auto reportId = value[0];
      handleReportIDMap[handle] = reportId;
    }
    HandleReportIDMapCache[client->getPeerAddress()] = handleReportIDMap;
  }
}

std::vector<NimBLERemoteCharacteristic*> getHIDReportCharacteristics(NimBLERemoteService* service) {
  auto characteristics = service->getCharacteristics(true);
  auto characteristicsHidReport = std::vector<NimBLERemoteCharacteristic*>();
  for (auto& c : *characteristics) {
    if (c->getUUID() == NimBLEUUID(CUUID_HID_REPORT_DATA)) {
      characteristicsHidReport.push_back(c);
    }
  }
  return characteristicsHidReport;
}

void subscribeToReport(NimBLEClient* client, NimBLERemoteCharacteristic* characteristic, const ReportItemList* reportItemList) {
  auto reportId = reportItemList->getReportID();
  auto usagePage = reportItemList->getUsagePage();
  auto usageID = reportItemList->getUsageID();

  auto isKeyboard = usagePage == static_cast<usagePage_t>(UsagePage::GENERIC_DESKTOP) &&
                    usageID == static_cast<usageID_t>(UsageIDGenericDesktop::KEYBOARD);
  auto isConsumerControl =
      usagePage == static_cast<usagePage_t>(UsagePage::CONSUMER) && usageID == static_cast<usageID_t>(UsageIDConsumer::CONSUMERCONTROL);
  auto isMouse =
      usagePage == static_cast<usagePage_t>(UsagePage::GENERIC_DESKTOP) && usageID == static_cast<usageID_t>(UsageIDGenericDesktop::MOUSE);

  if (isKeyboard || isConsumerControl) {
    auto ok = characteristic->subscribe(true, notifyCallbackKeyboardHIDReport);
    if (ok) {
      PS2BLE_LOGI(fmt::format("Subscribed to reportID: {}", reportId));
    } else {
      PS2BLE_LOGE(fmt::format("Failed to subscribe to reportID: {}", reportId));
    }
  }

  if (isMouse) {
    auto ok = characteristic->subscribe(true, notifyCallbackMouseHIDReport);
    if (ok) {
      PS2BLE_LOGI(fmt::format("Subscribed to reportID: {}", reportId));
    } else {
      PS2BLE_LOGE(fmt::format("Failed to subscribe to reportID: {}", reportId));
    }
  }
}

void subscribeHIDReportCharacteristics(NimBLEClient* client, const std::vector<NimBLERemoteCharacteristic*>& characteristicsHidReport) {
  for (auto& c : characteristicsHidReport) {
    auto desc = c->getDescriptor(NimBLEUUID(DUUID_HID_REPORT_REFERENCE));
    auto value = desc->readValue();
    if (value.size() != 2) continue;
    auto reportId = value[0];
    auto reportType = value[1];
    if (reportType != ESP_HID_REPORT_TYPE_INPUT) continue;
    auto reportMap = ReportMapCache[client->getPeerAddress()];
    auto inputReportItemLists = reportMap->getInputReportItemLists();
    for (auto& inputReportItemList : inputReportItemLists) {
      auto reportItemList = inputReportItemList.second;
      if (reportId == reportItemList->getReportID()) {
        subscribeToReport(client, c, reportItemList);
      }
    }
  }
}

void subscribeToHIDService(NimBLEClient* client) {
  NimBLERemoteService* service = client->getService(CUUID_HID_SERVICE);
  if (service == nullptr) {
    PS2BLE_LOGI("HID service not found");
    client->disconnect();
    return;
  }
  cacheReportMap(client, service);
  auto characteristicsHidReport = getHIDReportCharacteristics(service);
  cacheHandleReportIDMap(client, characteristicsHidReport);
  subscribeHIDReportCharacteristics(client, characteristicsHidReport);
}

bool getResetCount(std::uint8_t* resetCount) {
  auto ok = NVS.getBlob("resetCount", resetCount, 1);
  if (!ok) {
    PS2BLE_LOGE("Failed to read resetCount from NVS");
    return false;
  }
  return true;
}

void incrementResetCount() {
  std::uint8_t resetCount;
  auto ok = getResetCount(&resetCount);
  if (!ok) {
    resetCount = 0;
  }
  resetCount++;
  ok = NVS.setBlob("resetCount", &resetCount, 1);
  if (!ok) {
    PS2BLE_LOGE("Failed to write resetCount to NVS");
  }
}

void clearResetCount() {
  std::uint8_t resetCount = 0;
  auto ok = NVS.setBlob("resetCount", &resetCount, 1);
  if (!ok) {
    PS2BLE_LOGE("Failed to write resetCount to NVS");
  }
}

bool shouldRestorePs2InternalState() {
  constexpr std::uint8_t resetCountThreshold = 3;
  std::uint8_t resetCount;
  auto ok = getResetCount(&resetCount);
  if (!ok) {
    return false;
  }
  if (resetCount >= resetCountThreshold) {
    PS2BLE_LOGI(fmt::format("Reset counter: >= {}", resetCountThreshold));
    return false;
  } else {
    PS2BLE_LOGI(fmt::format("Reset counter: {}", resetCount));
    return true;
  }
}

void taskMouseBegin(void* arg) {
  mouse.set_byte_interval_micros(60);
  if (shouldRestorePs2InternalState()) {
    PS2BLE_LOGI("taskMouseBegin: restoring internal state");
    mouse.begin(true);
  } else {
    PS2BLE_LOGI("taskMouseBegin: not restoring internal state");
    mouse.begin(false);
  }
  vTaskDelete(NULL);
}

void taskKeyboardBegin(void* arg) {
  if (shouldRestorePs2InternalState()) {
    PS2BLE_LOGI("taskKeyboardBegin: restoring internal state");
    keyboard.begin(true);
  } else {
    PS2BLE_LOGI("taskKeyboardBegin: not restoring internal state");
    keyboard.begin(false);
  }
  vTaskDelete(NULL);
}

void wifiEventCallback(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_START:
      PS2BLE_LOGI("WiFi client started");
      break;
    case SYSTEM_EVENT_STA_STOP:
      PS2BLE_LOGI("WiFi client stopped");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      PS2BLE_LOGI("WiFi connected");
      PS2BLE_LOGI(fmt::format("SSID: {}", std::string(WiFi.SSID().c_str())));
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      PS2BLE_LOGI(fmt::format("IP address: {}", std::string(WiFi.localIP().toString().c_str())));
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      PS2BLE_LOGI("WiFi lost IP address");
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      PS2BLE_LOGI("WiFi disconnected");
      break;
    case SYSTEM_EVENT_AP_START:
      PS2BLE_LOGI("WiFi AP started");
      break;
    case SYSTEM_EVENT_AP_STOP:
      PS2BLE_LOGI("WiFi AP stopped");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      PS2BLE_LOGI("WiFi AP client connected");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      PS2BLE_LOGI("WiFi AP client disconnected");
      break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
      PS2BLE_LOGI("WiFi AP client IP assigned");
      break;
    default:
      break;
  }
}

constexpr auto LED_BUILTIN = 2;

void ledInit() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void ledOn() { digitalWrite(LED_BUILTIN, HIGH); }
void ledOff() { digitalWrite(LED_BUILTIN, LOW); }
void IRAM_ATTR ledToggle() { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); }

void apiBegin() {
  // handle GET to list bonded devices
  server.on("/api/bonded-devices", HTTP_GET, [](AsyncWebServerRequest* request) {
    auto doc = DynamicJsonDocument(4096);
    auto bondedDevices = doc.createNestedArray("bondedDevices");
    auto bondedNum = NimBLEDevice::getNumBonds();
    for (size_t i = 0; i < bondedNum; i++) {
      auto addr = NimBLEDevice::getBondedAddress(i);
      auto addrStr = std::string(addr);
      auto addrType = addr.getType();
      auto key = stripColon(addrStr);
      auto name = readDeviceNameFromNVS(addr);
      auto appearance = getAppearanceName(readAppearanceFromNVS(addr));
      auto clinet = NimBLEDevice::getClientByPeerAddress(addr);
      auto isConnected = clinet != nullptr && clinet->isConnected();
      auto bondedDevice = bondedDevices.createNestedObject();
      bondedDevice["address"] = addrStr;
      bondedDevice["addressType"] = addrType;
      bondedDevice["name"] = name;
      bondedDevice["appearance"] = appearance;
      bondedDevice["isConnected"] = isConnected;
    }
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });
  // handle POST to delete a bonded device
  auto handler = new AsyncCallbackJsonWebHandler("/api/bonded-devices/delete", [](AsyncWebServerRequest* request, JsonVariant& json) {
    StaticJsonDocument<256> response;
    response["deleted"] = false;
    response["message"] = "";
    const JsonObject& jsonObj = json.as<JsonObject>();
    auto addrStr = jsonObj["address"].as<String>();
    auto addrType = jsonObj["addressType"].as<std::uint8_t>();
    auto addr = NimBLEAddress(addrStr.c_str(), addrType);
    if (NimBLEDevice::isBonded(addr)) {
      PS2BLE_LOGI(fmt::format("Deleting bond for {}", std::string(addr)));
      auto ok = NimBLEDevice::deleteBond(addr);
      if (ok) {
        response["deleted"] = true;
      } else {
        response["message"] = "Failed to delete bond";
      }
    } else {
      PS2BLE_LOGI(fmt::format("Bond not found for {}", std::string(addr)));
      response["message"] = "Bond not found";
    }
    String responseStr;
    serializeJson(response, responseStr);
    request->send(200, "application/json", responseStr);
  });
  server.addHandler(handler);
  // handle GET to get scan mode
  server.on("/api/scan-mode", HTTP_GET, [](AsyncWebServerRequest* request) {
    auto doc = DynamicJsonDocument(256);
    doc["scanMode"] = static_cast<std::uint8_t>(DEFAULT_SCAN_MODE);
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });
  // handle POST to set scan mode
  handler = new AsyncCallbackJsonWebHandler("/api/scan-mode", [](AsyncWebServerRequest* request, JsonVariant& json) {
    StaticJsonDocument<256> response;
    response["ok"] = false;
    response["message"] = "";
    const JsonObject& jsonObj = json.as<JsonObject>();
    auto scanMode = jsonObj["scanMode"].as<std::uint8_t>();

    auto currentScanMode = DEFAULT_SCAN_MODE;
    auto ret = xQueuePeek(xQueueScanMode, &currentScanMode, 0);
    if (ret != pdTRUE) {
      PS2BLE_LOGE("xQueuePeek failed for xQueueScanMode");
    }

    if (currentScanMode == ScanMode::Disabled) {
      PS2BLE_LOGI("Scan disabled, do not overwrite scan mode");
      response["message"] = "Scan disabled";
    } else {
      PS2BLE_LOGI("Scan enabled, overwrite scan mode");
      auto isScanModeValid = isValidScanMode(scanMode);
      if (!isScanModeValid) {
        PS2BLE_LOGE(fmt::format("Invalid scan mode: {}", scanMode));
        response["message"] = "Invalid scan mode";
      } else {
        xQueueReset(xQueueLastConnectedDevice);
        auto mode = static_cast<ScanMode>(scanMode);
        ret = xQueueOverwrite(xQueueScanMode, &mode);
        if (ret != pdTRUE) {
          PS2BLE_LOGE("xQueueOverwrite failed for xQueueScanMode");
          response["message"] = "Failed to overwrite scan mode";
        } else {
          response["ok"] = true;
        }
      }
    }

    String responseStr;
    serializeJson(response, responseStr);
    request->send(200, "application/json", responseStr);
  });
  server.addHandler(handler);
  // handle GET to fetch last connected device
  server.on("/api/last-connected-device", HTTP_GET, [](AsyncWebServerRequest* request) {
    auto doc = DynamicJsonDocument(256);
    auto addr = NimBLEAddress();
    auto ret = xQueueReceive(xQueueLastConnectedDevice, &addr, 0);
    if (ret != pdTRUE) {
      PS2BLE_LOGI("No last connected device");
      doc["exists"] = false;
    } else {
      PS2BLE_LOGI(fmt::format("Last connected device: {}", std::string(addr)));
      auto lastConnectedDevice = doc.createNestedObject("lastConnectedDevice");
      auto addrStr = std::string(addr);
      auto name = readDeviceNameFromNVS(addr);
      auto appearance = getAppearanceName(readAppearanceFromNVS(addr));
      doc["exists"] = true;
      lastConnectedDevice["address"] = addrStr;
      lastConnectedDevice["name"] = name;
      lastConnectedDevice["appearance"] = appearance;
    }
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });
  // handle GET to fetch frontend files
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html").setCacheControl("public,max-age=31536000");
  server.begin();
  PS2BLE_LOGI("HTTP server started");
}

void setup() {
  Serial.begin(115200);

  PS2BLE_LOG_START();
  PS2DEV_LOG_START();

  // LED init
  ledInit();

  // NVS init
  PS2BLE_LOGI("Starting NVS");
  constexpr auto NVS_NAMESPACE = "ps2ble";
  auto ok = NVS.begin(NVS_NAMESPACE);
  if (!ok) {
    PS2BLE_LOGE("NVS init failed");
  }

  // Reset on first boot (looks weird but improves stability)
  std::uint8_t resetCount;
  ok = getResetCount(&resetCount);
  if (!ok) {
    PS2BLE_LOGE("Failed to read resetCount from NVS");
  } else {
    if (resetCount == 0) {
      PS2BLE_LOGI("Restarts in 3 seconds");
      incrementResetCount();
      delay(3000);
      ESP.restart();
    }
  }

  // Increment reset counter
  incrementResetCount();

  xTaskCreateUniversal(taskMouseBegin, "taskMouseBegin", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(taskKeyboardBegin, "taskKeyboardBegin", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);

  PS2BLE_LOGI("Starting NimBLE HID Client");
  NimBLEDevice::init("ps2ble");
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
  NimBLEDevice::setSecurityAuth(true, true, true);
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);

  // print bonded devices
  auto bondedNum = NimBLEDevice::getNumBonds();
  PS2BLE_LOGI(fmt::format("Number of bonded devices: {}", bondedNum));
  for (size_t i = 0; i < bondedNum; i++) {
    auto addr = NimBLEDevice::getBondedAddress(i);
    auto addrStr = std::string(addr);
    PS2BLE_LOGI(fmt::format("Bonded device {}: {}", i, addrStr));
  }

  // If reset counter is 5, start SoftAP and Web interface.
  ok = getResetCount(&resetCount);
  if (!ok) {
    PS2BLE_LOGE("Failed to read resetCount from NVS");
  } else if (resetCount == 5) {
    PS2BLE_LOGI("Starting LittleFS");
    LittleFS.begin();

    PS2BLE_LOGI("Starting WiFi Soft-AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY, AP_SUBNET);
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    PS2BLE_LOGI("Starting HTTP server");
    apiBegin();

    auto timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &ledToggle, true);
    timerAlarmWrite(timer, 500000, true);
    timerAlarmEnable(timer);
  }

  xQueueScanMode = xQueueCreate(1, sizeof(ScanMode));
  xQueueDeviceToConnect = xQueueCreate(9, sizeof(NimBLEAdvertisedDevice*));
  xQueueLastConnectedDevice = xQueueCreate(1, sizeof(NimBLEAddress));

  xTaskCreateUniversal(taskScan, "taskScan", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(taskConnect, "taskConnect", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);

  auto mode = DEFAULT_SCAN_MODE;
  auto ret = xQueueOverwrite(xQueueScanMode, &mode);
  if (ret != pdTRUE) {
    PS2BLE_LOGE("xQueueOverwrite failed for xQueueScanMode");
  }

  ledOn();

  // clear reset counter
  delay(5000);
  clearResetCount();
}

void loop() {
  delay(10000);
  // Monitor memory usage for debugging
  PS2BLE_LOGD(
      fmt::format("Free heap: {}/{} ({:.2f}%)", ESP.getFreeHeap(), ESP.getHeapSize(), ESP.getFreeHeap() * 100.0 / ESP.getHeapSize()));
}
