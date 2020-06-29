#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Iot/Aircon/Aircon.h>

WiFiClient wiFiClient;
Aircon *aircon = nullptr;

void setup() {
    Serial.begin(9600);
    Iot::setupWifi();
    aircon = new Aircon(wiFiClient);
}

void loop() {
    aircon->update();
    delay(2000);
}