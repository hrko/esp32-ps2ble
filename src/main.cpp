// clang-format off
#include <fmt/core.h>
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
#include <esp32-ps2dev.h>

AsyncWebServer server(80);
esp32_ps2dev::PS2Mouse mouse(17, 16);
esp32_ps2dev::PS2Keyboard keyboard(19, 18);

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
          // save the device name to NVS
          auto name = advertisedDevice->getName();
          if (name != "") {
            auto addr = advertisedDevice->getAddress();
            auto key = stripColon(addr.toString());
            auto ok = NVS.setString(key.c_str(), name.c_str());
            if (!ok) {
              PS2BLE_LOGE("Failed to save bonded device name to NVS");
            } else {
              PS2BLE_LOGI(fmt::format("Saved bonded device name to NVS: {} = {}", addr.toString(), name));
            }
          }
          // then, send client to subscribe task
          xQueueSend(xQueueClientToSubscribe, &client, portMAX_DELAY);
          continue;
        }
        delay(200);
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
        auto packet = esp32_ps2dev::PS2Packet();
        std::memcpy(packet.data, scanCode->getCode()->begin(), scanCode->getCode()->size());
        packet.len = scanCode->getCode()->size();
        keyboard.send_packet(packet);
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
        auto packet = esp32_ps2dev::PS2Packet();
        std::memcpy(packet.data, scanCode->getCode()->begin(), scanCode->getCode()->size());
        packet.len = scanCode->getCode()->size();
        keyboard.send_packet(packet);
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
      auto packet = esp32_ps2dev::PS2Packet();
      std::memcpy(packet.data, scanCode->getCode()->begin(), scanCode->getCode()->size());
      packet.len = scanCode->getCode()->size();
      keyboard.send_packet(packet);
    }
  }
  // update last report
  LastKeyboardReport[{addr, reportID}] = report;
  PS2BLE_LOGI(report.toString());
}

void notifyCallbackMouseHIDReport(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  auto addr = pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress();
  auto handle = pRemoteCharacteristic->getHandle();
  auto reportID = HandleReportIDMapCache[addr][handle];
  auto reportMap = ReportMapCache[addr];
  auto reportItemList = reportMap->getInputReportItemList(reportID);
  auto report = decodeMouseInputReport(pData, *reportItemList);
  mouse.move(report.x, -report.y, report.wheelVertical);
  for (size_t i = 0; i < 5; i++) {
    if (report.isButtonPressed[i]) {
      mouse.press(static_cast<esp32_ps2dev::PS2Mouse::Button>(i));
    } else {
      mouse.release(static_cast<esp32_ps2dev::PS2Mouse::Button>(i));
    }
  }
  PS2BLE_LOGI(report.toString());
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
          if (value.size() != 2) continue;
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
        if (value.size() != 2) continue;
        auto reportId = value[0];
        auto reportType = esp_hid_report_type_str(value[1]);
        // only print where report_type is INPUT
        if (value[1] != ESP_HID_REPORT_TYPE_INPUT) continue;
        auto serialOutput = fmt::format("handle: 0x{:02X}, report_id: {}, report_type: {}", handle, reportId, reportType);
        PS2BLE_LOGD(serialOutput);
      }

      // subscribe keyboard hid report characteristic
      for (auto& c : characteristicsHidReport) {
        auto desc = c->getDescriptor(NimBLEUUID(DUUID_HID_REPORT_REFERENCE));
        auto value = desc->readValue();
        if (value.size() != 2) continue;
        auto reportId = value[0];
        auto reportType = value[1];
        // only subscribe where report_type is INPUT
        if (reportType != ESP_HID_REPORT_TYPE_INPUT) continue;
        auto reportMap = ReportMapCache[client->getPeerAddress()];
        auto inputReportItemLists = reportMap->getInputReportItemLists();
        // search for corresponding reportItemList
        for (auto& inputReportItemList : inputReportItemLists) {
          auto reportItemList = inputReportItemList.second;
          if (reportId == reportItemList->getReportID()) {
            auto isKeyboard = reportItemList->getUsagePage() == static_cast<usagePage_t>(UsagePage::GENERIC_DESKTOP) &&
                              reportItemList->getUsageID() == static_cast<usageID_t>(UsageIDGenericDesktop::KEYBOARD);
            auto isConsumerControl = reportItemList->getUsagePage() == static_cast<usagePage_t>(UsagePage::CONSUMER) &&
                                     reportItemList->getUsageID() == static_cast<usageID_t>(UsageIDConsumer::CONSUMERCONTROL);
            auto isMouse = reportItemList->getUsagePage() == static_cast<usagePage_t>(UsagePage::GENERIC_DESKTOP) &&
                           reportItemList->getUsageID() == static_cast<usageID_t>(UsageIDGenericDesktop::MOUSE);
            if (isKeyboard || isConsumerControl) {
              auto result = c->subscribe(true, notifyCallbackKeyboardHIDReport);
              if (result) {
                auto serialOutput = fmt::format("Subscribed to reportID: {}", reportId);
                PS2BLE_LOGI(serialOutput);
              } else {
                auto serialOutput = fmt::format("Failed to subscribe to reportID: {}", reportId);
                PS2BLE_LOGE(serialOutput);
              }
            }
            if (isMouse) {
              auto result = c->subscribe(true, notifyCallbackMouseHIDReport);
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
  }
  vTaskDelete(NULL);
}

void taskMouseBegin(void* arg) {
  mouse.begin();
  vTaskDelete(NULL);
}

void taskKeyboardBegin(void* arg) {
  keyboard.begin();
  vTaskDelete(NULL);
}

void setup() {
  xTaskCreateUniversal(taskMouseBegin, "taskMouseBegin", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(taskKeyboardBegin, "taskKeyboardBegin", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);

  Serial.begin(115200);

  PS2BLE_LOG_START();

  // LittleFS init
  PS2BLE_LOGI("Starting LittleFS");
  LittleFS.begin();

  // NVS init
  PS2BLE_LOGI("Starting NVS");
  constexpr auto NVS_NAMESPACE = "ps2ble";
  auto ok = NVS.begin(NVS_NAMESPACE);
  if (!ok) {
    PS2BLE_LOGE("NVS init failed");
  }

  // PS2BLE_LOGI("Starting WiFi Soft-AP");
  // WiFi.mode(WIFI_AP);
  // WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY, AP_SUBNET);
  // WiFi.softAP(AP_SSID, AP_PASSWORD);

  PS2BLE_LOGI("Starting WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    PS2BLE_LOGI("Waiting for WiFi connection");
  }
  auto ipStr = std::string(WiFi.localIP().toString().c_str());
  PS2BLE_LOGI(fmt::format("WiFi connected, IP address: {}", ipStr));

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
      auto name = NVS.getString(key.c_str());
      if (name == nullptr) {
        name = "";
      }
      auto clinet = NimBLEDevice::getClientByPeerAddress(addr);
      auto isConnected = clinet != nullptr && clinet->isConnected();
      auto bondedDevice = bondedDevices.createNestedObject();
      bondedDevice["address"] = addrStr;
      bondedDevice["addressType"] = addrType;
      bondedDevice["name"] = name;
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
  // handle GET to fetch frontend files
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html").setCacheControl("public,max-age=31536000");
  server.begin();
  PS2BLE_LOGI(fmt::format("HTTP server started at http://{}", ipStr));

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

void loop() {
  delay(1000);
  // serialPrintln(fmt::format("Free heap: {}", ESP.getFreeHeap()));
}
