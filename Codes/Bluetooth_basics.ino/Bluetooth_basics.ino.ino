
/*

    Bluetooth - It uses a piconet framework to establish connection. It connects in master -slave mode, where slave can send data only to master and not 
    ohter slaves. 

    Bluetooth vs BLE : 

    ->Bluetooth also known as bluetooth classic is used for conitnuous streaming; It is generally meant for Large data transfer like audio or video files
    A bluetooth master can connect upto 7 slaves
    Data speed; 1MBPS - 3MBPS

    ->BLE AKA Bluetooth Low Energy is meant for low volume, burst data transfer which not only decreases the bandwidth but also the power consumption.
    Unlimited connections
    Data Speed; 125KBPS - 2MBPS

    This code is to create a central place to keep my application as well as the explanation in a nutshell
*/
/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h> //These are the headers needed for creating a BLE server 
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

/*

    UUIDs are 128-bit wide ID which are used to identify devices. It stands for Universally Unique ID. Here we are creating Service UUID and its CHARACTERISTIC's UUID

    Services && Characteristics :-

    GATT defines the format how BLE devices communicate:
    The topmost layer consists of profile which itself is a collection of many services
    A service itself is a collection of several Characteristics 
    Characteristics have their own properties, values && user defined or pre-defined no. of descriptor

    A Service can be User defined or can be from a predefined specification
    Each service has its own UUID. Some Services may have pre-defined UUIDs

*/
#define SERVICE_UUID       "c8ceec9-1ad4-4ddf-adaf-d4747c84b6d2"
#define CHARACTERISTIC_UUID "ff9835-e827-43c8-b88a-75b4c9903f9d"

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("Nalin's ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("I am right here!");
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
