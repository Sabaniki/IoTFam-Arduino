#include "Iot.h"

#include <utility>

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
    while (!Iot::mqtt.connected()) {
        Serial.print("Attempting MQTT connection... ");
        if (Iot::mqtt.connect(Iot::name)) {
            Serial.println("connected");
            char noticeMessage[64];
            snprintf(noticeMessage, 64, "from %s ----reconnect", name);
            mqtt.publish("server", noticeMessage);
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqtt.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

Iot::Iot(char *name, char *topic, WiFiClient &wiFiClient) :
        mqtt(wiFiClient),
        name(name),
        topic(topic) {
    mqtt.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
    mqtt.setCallback(defaultCallback);
    while (!mqtt.connected()) reconnect();
}

void Iot::sendMessage(Iot &messageTo, char *message) {
    // 別のESPに送信する処理
    // 同じ内容をラズパイにも送信する処理
}

void Iot::setCallback(std::function<void(char *, uint8_t *, unsigned int)> callback) {
    mqtt.setCallback(std::move(callback));
}

