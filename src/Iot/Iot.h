
/*
 * このクラスのインスタンスを作るよりも先にWiFiの諸々の設定をしてから使わないとダメです。
 * その部分もこのクラスに埋め込むことはできるけど、全体を通してこれ以外にもWiFiを使う部分が出ると良くない感じになるので。
 */

#ifndef IOTFAM_IOT_H
#define IOTFAM_IOT_H


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"
class Iot : private PubSubClient {
private:
    // char* topic;
    char* name;
    void reconnect();

public:
    Iot(char* name, WiFiClient& client);
};


#endif //IOTFAM_IOT_H
