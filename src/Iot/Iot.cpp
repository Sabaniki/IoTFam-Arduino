#include "Iot.h"

static void defaultCallback(char *_topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(_topic);
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

Iot::Iot(char *name,  WiFiClient &wiFiClient) :
        PubSubClient(wiFiClient),
        name(name) {
    PubSubClient::setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
    PubSubClient::setCallback(defaultCallback);
    while (!PubSubClient::connected()) Iot::reconnect();
}

void Iot::sendMessage(Iot &messageTo, char *message) {
    // 別のESPに送信する処理
    // 同じ内容をラズパイにも送信する処理
}

void Iot::publish(const char *topic, const char* payload) {
    PubSubClient::publish(topic, payload);
//    PubSubClient::publish()
}

