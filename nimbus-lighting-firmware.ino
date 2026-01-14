#include <Arduino.h>
#include "LEDStrip.h"
#include "config.h"
#include "UDPClient.h"

#define ARRAY_LEN(x) (int) sizeof(x) / (int)sizeof(x[0])
#define ID 0
#define LED_COUNT 100

ROBO::LEDStrip strips[] = {
    ROBO::LEDStrip(D4),
};


auto udpClient = ROBO::UDPClient(ID);

void setup() {
    Serial.begin(115200);
    delay(250);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.print("Available IDs:");
    for (int i = 0; i < ARRAY_LEN(strips); ++i) {
        strips[i].turnOff();
        Serial.print(", ");
    }
    Serial.print("\n");
    udpClient.init(WIFI_SSID,WIFI_PASSWORD, UDP_PORT);
    pinMode(D4, OUTPUT);
}

void loop() {
    const auto cmd = udpClient.get();
    for (int i = 0; i < ARRAY_LEN(strips); ++i) {
        if (cmd[i])
            strips[i].turnOn();
        else
            strips[i].turnOff();
    }

    // digitalWrite(D4,HIGH);
    yield();
}
