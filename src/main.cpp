#include <Arduino.h>
#include <NimBLEDevice.h>

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

const char HID_SERVICE[] = "1812";
const char HID_INFORMATION[] = "2A4A";
const char HID_REPORT_MAP[] = "2A4B";
const char HID_CONTROL_POINT[] = "2A4C";
const char HID_REPORT_DATA[] = "2A4D";

void scanEndedCB(NimBLEScanResults results);

static NimBLEAdvertisedDevice* advDevice;

static bool doConnect = false;
static uint32_t scanTime = 0; /** 0 = scan forever */

class ClientCallbacks : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) {
    Serial.println("Connected");
    pClient->updateConnParams(120, 120, 0, 60);
  };

  void onDisconnect(NimBLEClient* pClient) {
    Serial.print(pClient->getPeerAddress().toString().c_str());
    Serial.println(" Disconnected - Starting scan");
    NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
  };

  bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {
    return true;
  };

  uint32_t onPassKeyRequest() {
    Serial.println("Client Passkey Request");
    return 123456;
  };

  bool onConfirmPIN(uint32_t pass_key) {
    Serial.print("The passkey YES/NO number: ");
    Serial.println(pass_key);
    return true;
  };

  void onAuthenticationComplete(ble_gap_conn_desc* desc) {
    if (!desc->sec_state.encrypted) {
      Serial.println("Encrypt connection failed - disconnecting");
      NimBLEDevice::getClientByID(desc->conn_handle)->disconnect();
      return;
    }
  };
};

class AdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    if ((advertisedDevice->getAdvType() == BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD) ||
        (advertisedDevice->getAdvType() == BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD) ||
        (advertisedDevice->haveServiceUUID() && advertisedDevice->isAdvertisingService(NimBLEUUID(HID_SERVICE)))) {
      Serial.printf("onResult: AdvType= %d\r\n", advertisedDevice->getAdvType());
      Serial.print("Advertised HID Device found: ");
      Serial.println(advertisedDevice->toString().c_str());

      NimBLEDevice::getScan()->stop();
      advDevice = advertisedDevice;
      doConnect = true;
    }
  };
};

void notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  std::string str = (isNotify == true) ? "Notification" : "Indication";
  str += " from ";
  str += std::string(pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress());
  str += ": Service = " + std::string(pRemoteCharacteristic->getRemoteService()->getUUID());
  str += ", Characteristic = " + std::string(pRemoteCharacteristic->getUUID());
  str += ", Handle = 0x";
  Serial.print(str.c_str());
  Serial.print(pRemoteCharacteristic->getHandle());
  Serial.print(", Value = ");
  for (size_t i = 0; i < length; i++) {
    Serial.print(pData[i], HEX);
    Serial.print(',');
  }
  Serial.print(' ');
  Serial.println();
}

void scanEndedCB(NimBLEScanResults results) { Serial.println("Scan Ended"); }

static ClientCallbacks clientCB;

bool connectToServer() {
  NimBLEClient* pClient = nullptr;
  bool reconnected = false;

  Serial.printf("Client List Size: %d\r\n", NimBLEDevice::getClientListSize());
  if (NimBLEDevice::getClientListSize()) {
    pClient = NimBLEDevice::getClientByPeerAddress(advDevice->getAddress());
    if (pClient) {
      Serial.println("Reconnect try");
      if (!pClient->connect(advDevice, false)) {
        Serial.println("Reconnect failed");
        return false;
      }
      Serial.println("Reconnected client");
      reconnected = true;
    }
    else {
      pClient = NimBLEDevice::getDisconnectedClient();
    }
  }

  if (!pClient) {
    if (NimBLEDevice::getClientListSize() >= NIMBLE_MAX_CONNECTIONS) {
      Serial.println("Max clients reached - no more connections available");
      return false;
    }

    pClient = NimBLEDevice::createClient();

    Serial.println("New client created");

    pClient->setClientCallbacks(&clientCB, false);
    pClient->setConnectionParams(12, 12, 0, 51);
    pClient->setConnectTimeout(5);

    if (!pClient->connect(advDevice)) {
      NimBLEDevice::deleteClient(pClient);
      Serial.println("Failed to connect, deleted client");
      return false;
    }
  }

  if (!pClient->isConnected()) {
    if (!pClient->connect(advDevice)) {
      Serial.println("Failed to connect");
      return false;
    }
  }

  Serial.print("Connected to: ");
  Serial.println(pClient->getPeerAddress().toString().c_str());
  Serial.print("RSSI: ");
  Serial.println(pClient->getRssi());

  NimBLERemoteService* pSvc = nullptr;
  NimBLERemoteCharacteristic* pChr = nullptr;
  NimBLERemoteDescriptor* pDsc = nullptr;

  pSvc = pClient->getService(HID_SERVICE);
  if (pSvc) {
    if (!reconnected) {
      pChr = pSvc->getCharacteristic(HID_REPORT_MAP);
      if (pChr) {
        Serial.print("HID_REPORT_MAP ");
        if (pChr->canRead()) {
          std::string value = pChr->readValue();
          Serial.print(pChr->getUUID().toString().c_str());
          Serial.print(" Value: ");
          uint8_t* p = (uint8_t*)value.data();
          for (size_t i = 0; i < value.length(); i++) {
            Serial.print(p[i], HEX);
            Serial.print(',');
          }
          Serial.println();
        }
      } else {
        Serial.println("HID REPORT MAP char not found.");
      }
    }

    std::vector<NimBLERemoteCharacteristic*>* charvector;
    charvector = pSvc->getCharacteristics(true);
    for (auto& it : *charvector) {
      if (it->getUUID() == NimBLEUUID(HID_REPORT_DATA)) {
        Serial.println(it->toString().c_str());
        if (it->canNotify()) {
          if (!it->subscribe(true, notifyCB)) {
            /** Disconnect if subscribe failed */
            Serial.println("subscribe notification failed");
            pClient->disconnect();
            return false;
          }
        }
      }
    }
  }
  Serial.println("Done with this device!");
  return true;
}

void setup() {
  Serial.begin(115200);

  Serial.println("Starting NimBLE HID Client");
  NimBLEDevice::init("ps2ble");

  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
  NimBLEDevice::setSecurityAuth(true, true, true);

  NimBLEDevice::setPower(ESP_PWR_LVL_P9);

  NimBLEScan* pScan = NimBLEDevice::getScan();

  pScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());

  pScan->setInterval(45);
  pScan->setWindow(15);

  pScan->setActiveScan(true);
  pScan->start(scanTime, scanEndedCB);
}

void loop() {
  if (!doConnect) return;

  doConnect = false;

  if (connectToServer()) {
    Serial.println("Success! we should now be getting notifications!");
  } else {
    Serial.println("Failed to connect, starting scan");
    NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
  }
}