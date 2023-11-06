// clang-format off
#include <fmt/core.h>
#include <Arduino.h>
#include <NimBLEDevice.h>
// clang-format on

#include <cstdio>
extern "C" {
#include <esp_hid_common.h>
}

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
QueueHandle_t xQueueSerialOutput;
QueueHandle_t xQueueDeviceToConnect;
QueueHandle_t xQueueClientToSubscribe;

void serialPrint(std::string str);
void serialPrint(const char* str);
void serialPrint(char c);
void serialPrintln(std::string str);
void serialPrintln(const char* str);
void serialPrintln(char c);
void serialPrintln(void);

class ClientCallbacks : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) {
    auto output = fmt::format("Connected to: {}", pClient->getPeerAddress().toString());
    serialPrintln(output);
    // pClient->updateConnParams(120, 120, 0, 60);
    serialPrintln("Triggering scan");
    auto mode = ScanMode::NewDeviceAndBoundedDevice;
    xQueueSend(xQueueScanMode, &mode, portMAX_DELAY);
  };

  void onDisconnect(NimBLEClient* pClient) {
    auto output = fmt::format("Disconnected from: {}", pClient->getPeerAddress().toString());
    serialPrintln(output);
    serialPrintln("Triggering scan");
    auto mode = ScanMode::NewDeviceAndBoundedDevice;
    xQueueSend(xQueueScanMode, &mode, portMAX_DELAY);
  };

  bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) { return true; };

  uint32_t onPassKeyRequest() {
    serialPrintln("Client Passkey Request");
    return 123456;
  };

  bool onConfirmPIN(uint32_t pass_key) {
    auto output = fmt::format("The passkey YES/NO number: {}", pass_key);
    serialPrintln(output);
    return true;
  };

  void onAuthenticationComplete(ble_gap_conn_desc* desc) {
    serialPrintln("Pairing completed");
    if (!desc->sec_state.encrypted) {
      serialPrintln("WARNING: Link is not encrypted");
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
  serialPrintln(str);
}

static ClientCallbacks clientCB;

void taskSerial(void* arg) {
  while (true) {
    std::string* str;
    if (xQueueReceive(xQueueSerialOutput, &str, portMAX_DELAY) == pdTRUE) {
      Serial.print(str->c_str());
      delete str;
    }
  }
}

void serialPrint(std::string str) {
  auto strCopy = new std::string(str);
  xQueueSend(xQueueSerialOutput, &strCopy, portMAX_DELAY);
}
void serialPrint(const char* str) { serialPrint(std::string(str)); }
void serialPrint(char c) { serialPrint(std::string(1, c)); }
void serialPrintln(void) { serialPrint("\r\n"); }
void serialPrintln(std::string str) { serialPrint(str + "\r\n"); }
void serialPrintln(const char* str) { serialPrint(std::string(str) + "\r\n"); }
void serialPrintln(char c) { serialPrint(std::string(1, c) + "\r\n"); }

bool isDirectedAdvertisement(NimBLEAdvertisedDevice* advertisedDevice) {
  auto advType = advertisedDevice->getAdvType();
  if (advType == BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD || advType == BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD) {
    return true;
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
    if (isDirectedAdvertisement(advertisedDevice)) return;
    if (isAdvertisingHIDService(advertisedDevice)) {
      xQueueSend(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY);
    }
  };
};

class AdvertisedDeviceCallbacksNewDeviceAndBoundedDevice : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    if (isDirectedAdvertisement(advertisedDevice) || isAdvertisingHIDService(advertisedDevice)) {
      xQueueSend(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY);
    }
  };
};

class AdvertisedDeviceCallbacksBoundedDeviceOnly : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    if (isDirectedAdvertisement(advertisedDevice)) {
      xQueueSend(xQueueDeviceToConnect, &advertisedDevice, portMAX_DELAY);
    }
  };
};

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

      scan->start(0, nullptr);
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

void taskSubscribe(void* arg) {
  NimBLEClient* client;
  while (true) {
    if (xQueueReceive(xQueueClientToSubscribe, &client, portMAX_DELAY) == pdTRUE) {
      NimBLERemoteService* service = nullptr;
      NimBLERemoteCharacteristic* characteristic = nullptr;

      service = client->getService(CUUID_HID_SERVICE);
      if (service == nullptr) {
        serialPrintln("HID service not found");
        client->disconnect();
        continue;
      }

      // print report map for debug
      characteristic = service->getCharacteristic(CUUID_HID_REPORT_MAP);
      if (characteristic == nullptr) {
        serialPrintln("HID report map characteristic not found");
      } else {
        auto serialOutput = std::string("HID_REPORT_MAP \r\n");
        if (characteristic->canRead()) {
          auto value = characteristic->readValue();
          auto rawReportMap = value.data();
          auto rawReportMapLength = value.length();
          auto reportMap = esp_hid_parse_report_map(rawReportMap, rawReportMapLength);
          for (size_t i = 0; i < reportMap->reports_len; i++) {
            auto reportItem = reportMap->reports[i];
            auto mapIndex = reportItem.map_index;
            auto reportId = reportItem.report_id;
            auto reportType = esp_hid_report_type_str(reportItem.report_type);
            auto protocolMode = esp_hid_protocol_mode_str(reportItem.protocol_mode);
            auto usage = esp_hid_usage_str(reportItem.usage);
            auto valueLen = reportItem.value_len;

            serialOutput += fmt::format("map_index: {}, report_id: {}, report_type: {}, protocol_mode: {}, usage: {}, value_len: {}\r\n",
                                        mapIndex, reportId, reportType, protocolMode, usage, valueLen);
          }
          esp_hid_free_report_map(reportMap);
        }
        serialPrint(serialOutput);
      }

      auto characteristics = service->getCharacteristics(true);
      auto characteristicsHidReport = std::vector<NimBLERemoteCharacteristic*>();
      for (auto& c : *characteristics) {
        if (c->getUUID() == NimBLEUUID(CUUID_HID_REPORT_DATA)) {
          characteristicsHidReport.push_back(c);
        }
      }

      // print hid report characteristic for debug
      for (auto& c : characteristicsHidReport) {
        auto handle = c->getHandle();
        auto desc = c->getDescriptor(NimBLEUUID(DUUID_HID_REPORT_REFERENCE));
        auto value = desc->readValue();
        auto reportId = value[0];
        auto reportType = esp_hid_report_type_str(value[1]);
        auto serialOutput = fmt::format("handle: {}, report_id: {}, report_type: {}", handle, reportId, reportType);
        serialPrintln(serialOutput);
      }

      // subscribe all hid report characteristic for debug
      for (auto& c : characteristicsHidReport) {
        if (c->canNotify()) {
          c->subscribe(true, notifyCB);
        }
      }
    }
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);

  xQueueSerialOutput = xQueueCreate(20, sizeof(std::string*));
  xTaskCreateUniversal(taskSerial, "taskSerial", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);

  serialPrintln("Starting NimBLE HID Client");
  NimBLEDevice::init("ps2ble");
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
  NimBLEDevice::setSecurityAuth(true, true, true);
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);

  xQueueScanMode = xQueueCreate(1, sizeof(uint8_t));
  xQueueDeviceToConnect = xQueueCreate(9, sizeof(NimBLEAdvertisedDevice*));
  xQueueClientToSubscribe = xQueueCreate(9, sizeof(NimBLEClient*));

  xTaskCreateUniversal(taskScan, "taskScan", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(taskConnect, "taskConnect", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(taskSubscribe, "taskSubscribe", 4096, nullptr, 1, nullptr, CONFIG_ARDUINO_RUNNING_CORE);

  auto mode = ScanMode::NewDeviceAndBoundedDevice;
  auto ret = xQueueSend(xQueueScanMode, &mode, portMAX_DELAY);
  if (ret != pdTRUE) {
    serialPrintln("xQueueSend failed for xQueueScanMode");
  }
}

void loop() { delay(1000); }
