// clang-format off
#include <fmt/core.h>
#include <Arduino.h>
#include <NimBLEDevice.h>
// clang-format on

#include <cstdio>

#include "hid/keyboard.hpp"
#include "hid/report_map.hpp"
#include "key_translate.hpp"
#include "logging.hpp"
extern "C" {
#include <esp_hid_common.h>
}

#include <map>

const uint16_t APPEARANCE_HID_GENERIC = 0x3C0;
const uint16_t APPEARANCE_HID_KEYBOARD = 0x3C1;
const uint16_t APPEARANCE_HID_MOUSE = 0x3C2;
const uint16_t APPEARANCE_HID_JOYSTICK = 0x3C3;
const uint16_t APPEARANCE_HID_GAMEPAD = 0x3C4;
const uint16_t APPEARANCE_HID_DIGITIZER_TABLET = 0x3C5;
const uint16_t APPEARANCE_HID_CARD_READER = 0x3C6;
const uint16_t APPEARANCE_HID_DIGITAL_PEN = 0x3C7;
const uint16_t APPEARANCE_HID_BARCODE_SCANNER = 0x3C8;
const uint16_t APPEARANCE_HID_TOUCHPAD = 0x3C9;
const uint16_t APPEARANCE_HID_PRESENTATION_REMOTE = 0x3CA;

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
};

QueueHandle_t xQueueScanMode;
QueueHandle_t xQueueDeviceToConnect;
QueueHandle_t xQueueClientToSubscribe;

using HandleReportIDMap = std::map<uint16_t, reportID_t>;
std::map<NimBLEAddress, HandleReportIDMap> HandleReportIDMapCache;
std::map<NimBLEAddress, ReportMap*> ReportMapCache;

class ClientCallbacks : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) {
    auto output = fmt::format("Connected to: {}", pClient->getPeerAddress().toString());
    PS2BLE_LOGI(output);
    // pClient->updateConnParams(120, 120, 0, 60);
    PS2BLE_LOGI("Triggering scan");
    auto mode = ScanMode::NewDeviceAndBoundedDevice;
    xQueueSend(xQueueScanMode, &mode, portMAX_DELAY);
  };

  void onDisconnect(NimBLEClient* pClient) {
    auto output = fmt::format("Disconnected from: {}", pClient->getPeerAddress().toString());
    PS2BLE_LOGI(output);
    PS2BLE_LOGI("Triggering scan");
    auto mode = ScanMode::NewDeviceAndBoundedDevice;
    xQueueSend(xQueueScanMode, &mode, portMAX_DELAY);
  };

  bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) { return true; };

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

void notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  auto addr = std::string(pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress());
  auto svc = std::string(pRemoteCharacteristic->getRemoteService()->getUUID());
  auto chr = std::string(pRemoteCharacteristic->getUUID());
  auto handle = fmt::format("0x{:02X}", pRemoteCharacteristic->getHandle());
  auto value = std::string();
  for (size_t i = 0; i < length; i++) {
    fmt::format_to(std::back_inserter(value), "{:02X},", pData[i]);
  }
  auto str = fmt::format("{} from {} : Service = {}, Characteristic = {}, Handle = {}, Value = {}",
                         isNotify ? "Notification" : "Indication", addr, svc, chr, handle, value);
  PS2BLE_LOGI(str);
}

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
    }
  };
};

class AdvertisedDeviceCallbacksNewDeviceAndBoundedDevice : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    if (isBondedDevice(advertisedDevice) || isAdvertisingHIDService(advertisedDevice)) {
      xQueueSend(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY);
    }
  };
};

class AdvertisedDeviceCallbacksBoundedDeviceOnly : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    if (isBondedDevice(advertisedDevice)) {
      xQueueSend(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY);
    }
  };
};

void scanCompleteCB(NimBLEScanResults) {
  PS2BLE_LOGI("Scan complete");
  // auto mode = ScanMode::NewDeviceAndBoundedDevice;
  // xQueueSend(xQueueScanMode, &mode, portMAX_DELAY);
}

void taskScan(void* arg) {
  AdvertisedDeviceCallbacksNewDeviceOnly newDeviceOnlyCallbacks;
  AdvertisedDeviceCallbacksNewDeviceAndBoundedDevice newDeviceAndBoundedDeviceCallbacks;
  AdvertisedDeviceCallbacksBoundedDeviceOnly boundedDeviceOnlyCallbacks;
  NimBLEScan* scan = NimBLEDevice::getScan();
  scan->setInterval(45);
  scan->setWindow(15);
  scan->setActiveScan(true);
  ScanMode scanMode;

  while (true) {
    if (xQueueReceive(xQueueScanMode, &scanMode, portMAX_DELAY) == pdTRUE) {
      if (scan->isScanning()) {
        scan->stop();
      }

      switch (scanMode) {
        case ScanMode::NewDeviceOnly:
          scan->setAdvertisedDeviceCallbacks(&newDeviceOnlyCallbacks);
          break;
        case ScanMode::NewDeviceAndBoundedDevice:
          scan->setAdvertisedDeviceCallbacks(&newDeviceAndBoundedDeviceCallbacks);
          break;
        case ScanMode::BoundedDeviceOnly:
          scan->setAdvertisedDeviceCallbacks(&boundedDeviceOnlyCallbacks);
          break;
        default:
          continue;
      }

      scan->start(0, scanCompleteCB);
    }
  }
}

void taskConnect(void* arg) {
  NimBLEAdvertisedDevice* advertisedDevice;
  while (true) {
    if (xQueueReceive(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY) == pdTRUE) {
      // first, try to use the existing client to reduce the connection time
      NimBLEClient* client = NimBLEDevice::getClientByPeerAddress(advertisedDevice->getAddress());
      if (client != nullptr) {
        auto isConnected = client->connect(advertisedDevice, false);
        if (isConnected) {
          xQueueSend(xQueueClientToSubscribe, &client, portMAX_DELAY);
          continue;
        }
      }

      // if the existing client is not available, create a new client
      client = NimBLEDevice::createClient();
      client->setClientCallbacks(&clientCB, false);
      client->setConnectionParams(12, 12, 0, 51);
      client->setConnectTimeout(5);
      for (size_t i = 0; i < 3; i++) {  // try 3 times
        auto isConnected = client->connect(advertisedDevice);
        if (isConnected) {
          xQueueSend(xQueueClientToSubscribe, &client, portMAX_DELAY);
          continue;
        }
        delay(200);
      }
    }
  }
  vTaskDelete(NULL);
}

void notifyCallbackKeyboardHIDReport(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  auto addr = pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress();
  auto handle = pRemoteCharacteristic->getHandle();

  auto reportID = HandleReportIDMapCache[addr][handle];
  auto reportMap = ReportMapCache[addr];
  auto reportItemList = reportMap->getInputReportItemList(reportID);

  auto report = decodeKeyboardInputReport(pData, reportItemList);
  auto pressedKeys = report.getPressedKeys();

  auto output = fmt::format("reportID: {}, pressedKeys: ", reportID);
  for (auto& key : pressedKeys) {
    output += fmt::format("{},", key);
  }

  PS2BLE_LOGI(output);
}

void taskSubscribe(void* arg) {
  NimBLEClient* client;
  while (true) {
    if (xQueueReceive(xQueueClientToSubscribe, &client, portMAX_DELAY) == pdTRUE) {
      NimBLERemoteService* service = nullptr;
      NimBLERemoteCharacteristic* characteristic = nullptr;

      service = client->getService(CUUID_HID_SERVICE);
      if (service == nullptr) {
        PS2BLE_LOGI("HID service not found");
        client->disconnect();
        continue;
      }

      // print report map for debug
      characteristic = service->getCharacteristic(CUUID_HID_REPORT_MAP);
      if (characteristic == nullptr) {
        PS2BLE_LOGI("HID report map characteristic not found");
      } else {
        auto serialOutput = std::string("HID_REPORT_MAP \r\n");
        if (characteristic->canRead()) {
          auto value = characteristic->readValue();
          auto rawReportMap = value.data();
          auto rawReportMapLength = value.length();
          auto reportMap = esp_hid_parse_report_map(rawReportMap, rawReportMapLength);

          // print raw report map first
          serialOutput += fmt::format("raw report map: ");
          for (size_t i = 0; i < rawReportMapLength; i++) {
            serialOutput += fmt::format("0x{:02X},", rawReportMap[i]);
          }
          serialOutput += fmt::format("\r\n");

          for (size_t i = 0; i < reportMap->reports_len; i++) {
            auto reportItem = reportMap->reports[i];
            auto mapIndex = reportItem.map_index;
            auto reportId = reportItem.report_id;
            auto reportType = esp_hid_report_type_str(reportItem.report_type);
            auto protocolMode = esp_hid_protocol_mode_str(reportItem.protocol_mode);
            auto usage = esp_hid_usage_str(reportItem.usage);
            auto valueLen = reportItem.value_len;

            // only print where protocol_mode is REPORT and report_type is INPUT
            if (reportItem.protocol_mode != ESP_HID_PROTOCOL_MODE_REPORT) continue;
            if (reportItem.report_type != ESP_HID_REPORT_TYPE_INPUT) continue;
            serialOutput += fmt::format("map_index: {}, report_id: {}, report_type: {}, protocol_mode: {}, usage: {}, value_len: {}\r\n",
                                        mapIndex, reportId, reportType, protocolMode, usage, valueLen);
          }
          esp_hid_free_report_map(reportMap);

          // print report map using ReportMap class
          auto reportMap2 = ReportMap(rawReportMap, rawReportMapLength);
          serialOutput += reportMap2.toString();
        }
        PS2BLE_LOGD(serialOutput);
      }

      // Cache report map
      auto isReportMapCached = ReportMapCache.find(client->getPeerAddress()) != ReportMapCache.end();
      if (!isReportMapCached) {
        characteristic = service->getCharacteristic(CUUID_HID_REPORT_MAP);
        if (characteristic != nullptr) {
          auto value = characteristic->readValue();
          auto rawReportMap = value.data();
          auto rawReportMapLength = value.length();
          auto reportMap = new ReportMap(rawReportMap, rawReportMapLength);
          ReportMapCache[client->getPeerAddress()] = reportMap;
        }
      }

      auto characteristics = service->getCharacteristics(true);
      auto characteristicsHidReport = std::vector<NimBLERemoteCharacteristic*>();
      for (auto& c : *characteristics) {
        if (c->getUUID() == NimBLEUUID(CUUID_HID_REPORT_DATA)) {
          characteristicsHidReport.push_back(c);
        }
      }

      // Cache handle report id map
      auto isHandleReportIDMapCached = HandleReportIDMapCache.find(client->getPeerAddress()) != HandleReportIDMapCache.end();
      if (!isHandleReportIDMapCached) {
        auto handleReportIDMap = HandleReportIDMap();
        for (auto& c : characteristicsHidReport) {
          auto handle = c->getHandle();
          auto desc = c->getDescriptor(NimBLEUUID(DUUID_HID_REPORT_REFERENCE));
          auto value = desc->readValue();
          auto reportId = value[0];
          handleReportIDMap[handle] = reportId;
        }
        HandleReportIDMapCache[client->getPeerAddress()] = handleReportIDMap;
      }

      // print hid report characteristic for debug
      for (auto& c : characteristicsHidReport) {
        auto handle = c->getHandle();
        auto desc = c->getDescriptor(NimBLEUUID(DUUID_HID_REPORT_REFERENCE));
        auto value = desc->readValue();
        auto reportId = value[0];
        auto reportType = esp_hid_report_type_str(value[1]);
        // only print where report_type is INPUT
        if (value[1] != ESP_HID_REPORT_TYPE_INPUT) continue;
        auto serialOutput = fmt::format("handle: 0x{:02X}, report_id: {}, report_type: {}", handle, reportId, reportType);
        PS2BLE_LOGD(serialOutput);
      }

      // // subscribe all hid report characteristic for debug
      // for (auto& c : characteristicsHidReport) {
      //   if (c->canNotify()) {
      //     c->subscribe(true, notifyCB);
      //   }
      // }

      // subscribe keyboard hid report characteristic
      for (auto& c : characteristicsHidReport) {
        auto desc = c->getDescriptor(NimBLEUUID(DUUID_HID_REPORT_REFERENCE));
        auto value = desc->readValue();
        auto reportId = value[0];
        auto reportType = value[1];
        if (reportType != ESP_HID_REPORT_TYPE_INPUT) continue;
        auto reportMap = ReportMapCache[client->getPeerAddress()];
        auto inputReportItemLists = reportMap->getInputReportItemLists();
        // look for ReportItem with UsagePage:UsageID == 0x0001:0x0006 or 0x000c:0x0001
        for (auto& inputReportItemList : inputReportItemLists) {
          auto reportItemList = inputReportItemList.second;
          auto isKeyboard = reportItemList->getUsagePage() == static_cast<usagePage_t>(UsagePage::GENERIC_DESKTOP) &&
                            reportItemList->getUsageID() == static_cast<usageID_t>(UsageIDGenericDesktop::KEYBOARD);
          auto isConsumerControl = reportItemList->getUsagePage() == static_cast<usagePage_t>(UsagePage::CONSUMER) &&
                                   reportItemList->getUsageID() == static_cast<usageID_t>(UsageIDConsumer::CONSUMERCONTROL);
          if (!isKeyboard && !isConsumerControl) continue;
          if (reportId == reportItemList->getReportID() && c->canNotify()) {
            auto result = c->subscribe(true, notifyCallbackKeyboardHIDReport);
            if (result) {
              auto serialOutput = fmt::format("Subscribed to reportID: {}", reportId);
              PS2BLE_LOGI(serialOutput);
            } else {
              auto serialOutput = fmt::format("Failed to subscribe to reportID: {}", reportId);
              PS2BLE_LOGE(serialOutput);
            }
          }
        }
      }
    }
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);

  PS2BLE_LOG_START();

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

  xQueueScanMode = xQueueCreate(1, sizeof(uint8_t));
  xQueueDeviceToConnect = xQueueCreate(9, sizeof(NimBLEAdvertisedDevice*));
  xQueueClientToSubscribe = xQueueCreate(9, sizeof(NimBLEClient*));

  xTaskCreateUniversal(taskScan, "taskScan", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(taskConnect, "taskConnect", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(taskSubscribe, "taskSubscribe", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);

  auto mode = ScanMode::NewDeviceAndBoundedDevice;
  auto ret = xQueueSend(xQueueScanMode, &mode, portMAX_DELAY);
  if (ret != pdTRUE) {
    PS2BLE_LOGE("xQueueSend failed for xQueueScanMode");
  }
}

void loop() { delay(1000); }
