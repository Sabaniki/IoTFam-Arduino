#include "Aircon.h"

void Aircon::callback(char *topic, const byte *payload, unsigned int length) {
    if(strcmp(topic, "iot/temp") == 0){
        if ((int)payload[0] > 25) Serial.println("warm! turn on air-con!");
        else Serial.println("not warm. good.");
    }
}

Aircon::Aircon(WiFiClient &wiFiClient) : Iot("Aircon", wiFiClient) {
    setCallback(Aircon::callback);
}

void Aircon::update() {
    if (!connected()) reconnect();
    loop();
    subscribe("iot/temp");
}
