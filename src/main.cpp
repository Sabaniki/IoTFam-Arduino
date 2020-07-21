#include "Arduino.h"
#include <Iot/Thermometer/Thermometer.h>
#include <ESP8266WiFi.h>

WiFiClient wiFiClient;
Thermometer *thermometer = nullptr;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    Iot::setupWifi();
    thermometer = new Thermometer(wiFiClient);
}

void loop() {
    thermometer->update();
    delay(2000);
}