#include "Thermometer.h"

void Thermometer::callback(char *topic, const byte *payload, unsigned int length) {
    if ((int)payload[0] > 25) Serial.println("warm! turn on air-con!");
    else Serial.println("not warm. good.");
}

Thermometer::Thermometer(WiFiClient &wiFiClient) : Iot("Thermometer", wiFiClient), dht(DHTPIN, DHTTYPE) {
    Thermometer::dht.begin();
}

void Thermometer::update() {
    if (!dht.readTempAndHumidity(Thermometer::tempAndHumVal)){
        Serial.print("Humidity: ");
        Serial.print(Thermometer::tempAndHumVal[0]);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(Thermometer::tempAndHumVal[1]);
        Serial.println(" *C");
    }
    else {
        Serial.println("failed to read value");
    }
}
