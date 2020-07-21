#ifndef IOTFAM_THERMOMETER_H
#define IOTFAM_THERMOMETER_H


#include <Iot/Iot.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

#if defined(ARDUINO_ARCH_AVR)
#define debug  Serial

#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
#define debug  SerialUSB
#else
#define debug  Serial
#endif

class Thermometer: Iot {
private:
    static void callback(char *topic, const byte *payload, unsigned int length);
    float tempAndHumVal[2]{};
    DHT dht;

public:
    explicit Thermometer(WiFiClient &wiFiClient);
    void update() override;
};


#endif //IOTFAM_THERMOMETER_H
