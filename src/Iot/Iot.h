
/*
 * このクラスのインスタンスを作るよりも先にWiFiの諸々の設定をしてから使わないとダメ。
 * その部分もこのクラスに埋め込むことはできるけど、全体を通してこれ以外にもWiFiを使う部分が出ると良くない感じになるので。
 * (どうせ自分しか使わないのに誰に言ってるんだろうね)
 */

#ifndef IOTFAM_IOT_H
#define IOTFAM_IOT_H


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"
class Iot : public PubSubClient {
private:
    const char* name;

public:
    Iot(const char* name, WiFiClient& client);
    static void setupWifi();
    void reconnect();
    virtual void update() = 0;
};


#endif //IOTFAM_IOT_H
