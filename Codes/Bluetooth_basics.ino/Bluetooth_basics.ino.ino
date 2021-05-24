
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

#include <BLEDevice.h>  //These are the headers needed for creating a BLE server
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

#define SERVICE_UUID "1a2a3e3f-1fb5-459e-8fcc-c5c9c331924b"
#define CHARACTERISTIC_UUID "1a2a3e3f-1fb5-459e-8fcc-c5c9c331924b"  //"beb5483e-36e1-4688-b7f5-ea07061b26a8"

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();  // Defines a class and associates it with a callback function

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

  BLEDevice::init("ESP32");  //Used to name the esp32

  //BLEServer initialization
  BLEServer *pServer = BLEDevice::createServer();                       //Used to create a BLE server; It initializes the ESP32 as a BLE Server
  BLEService *pService = pServer->createService(SERVICE_UUID);          //Used to create a custom service with the mentioned UUID
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(  // Used to create Characteristic of the given custom service
    CHARACTERISTIC_UUID,                                                // This Characteristic has this CHARACTERISTIC_UUID
    BLECharacteristic::PROPERTY_READ |                                  // The service will have READ && WRITE Properties
      BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("I am right here!");     // Sets the value of the characteristic such that it is available to read from
  pCharacteristic->setCallbacks(new MyCallbacks());  // Sets the callback function to point ti the specified class MyCallbacks
  pService->start();                                 // Starts the custom service

  // This code is used to init advertising related variables
  // BLEAdvertising *pAdvertising = pServer2->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();  // Starts advertising the device to other slaves such that they can detect the device
                                  // connect to it and start communicating with it

  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
