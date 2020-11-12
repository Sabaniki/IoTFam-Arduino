#ifndef IOTFAM_AIRCON_H
#define IOTFAM_AIRCON_H


#include <Iot/Iot.h>
#include <IRremoteESP8266.h>

class Aircon : Iot {
private:
    static void callback(char *topic, const byte *payload, unsigned int length);

public:
    explicit Aircon(WiFiClient &wiFiClient);

    void update() override;
};


#endif //IOTFAM_AIRCON_H
