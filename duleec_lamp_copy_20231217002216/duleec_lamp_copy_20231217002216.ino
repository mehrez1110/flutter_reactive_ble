#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED_PIN 4
#define BLINK_PIN 2

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool blinkLED = false;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
            uint8_t byteValue = static_cast<uint8_t>(value[0]);
            if (byteValue == 1) {
                digitalWrite(LED_PIN, HIGH);
                Serial.println("LED turned ON");
            } else if (byteValue == 0) {
                digitalWrite(LED_PIN, LOW);
                Serial.println("LED turned OFF");
            }
            

            // Now 'byteValue' contains the unsigned byte value
            Serial.print("Received byte value: ");
            Serial.println(byteValue);
        
    }
};

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BLINK_PIN, OUTPUT);

    BLEDevice::init("duleec lamp");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService* pService = pServer->createService(BLEUUID("0000ffe0-0000-1000-8000-00805f9b34fb"));

    pCharacteristic = pService->createCharacteristic(
        BLEUUID("0000ffe1-0000-1000-8000-00805f9b34fb"),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(new MyCallbacks());

    pService->start();

    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    Serial.println("Waiting for a connection...");

    // Blink the pin until paired
    while (!deviceConnected) {
        digitalWrite(BLINK_PIN, HIGH);
        delay(500);
        digitalWrite(BLINK_PIN, LOW);
        delay(500);
    }
}

void loop() {
    if (deviceConnected) {
        // Your main code here
    }
}
