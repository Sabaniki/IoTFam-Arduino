#include "Iot.h"


static void defaultCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.print("payload (print as string): ");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
}

void Iot::reconnect() {
    while (!PubSubClient::connected()) {
        Serial.print("Attempting MQTT connection... ");
        if (PubSubClient::connect(Iot::name)) {
            Serial.println("connected");
            char noticeMessage[64];
            snprintf(noticeMessage, 64, "from %s ----reconnect", name);
            PubSubClient::publish("server", noticeMessage);
        } else {
            Serial.print("failed, rc=");
            Serial.print(state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

Iot::Iot(const char *name, WiFiClient &wiFiClient) :
        PubSubClient(wiFiClient),
        name(name) {
    PubSubClient::setServer(RASPI_IP, MQTT_SERVER_PORT);
    PubSubClient::setCallback(defaultCallback);
    while (!PubSubClient::connected()) Iot::reconnect();
}

void Iot::setupWifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

