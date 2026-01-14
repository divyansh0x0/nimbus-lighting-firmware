#include <Arduino.h>
#include "LEDStrip.h"
#include "config.h"
#include "UDPClient.h"
#include "LEDStripLoader.h"

#define ARRAY_LEN(x) (int) sizeof(x) / (int)sizeof(x[0])
#define ID_START 0
#define LED_COUNT 100

unsigned char id_count = BASE_ID_COUNT;

unsigned char getStripId() {
    return id_count++;
};
ROBO::LEDStrip strips[] = {
    ROBO::LEDStrip::create(LED_COUNT, getStripId()),
};
auto udpClient = ROBO::UDPClient(ID_START);

void setup() {
    Serial.begin(115200);
    delay(250);
    pinMode(LED_BUILTIN, OUTPUT);

    ROBO::LEDStripLoader<D4>::init(strips);


    Serial.print("Available IDs:");
    for (int i = 0; i < ARRAY_LEN(strips); ++i) {
        strips[i].turnOff();
        Serial.print(strips[i].getId());
        Serial.print(", ");
    }
    Serial.print("\n");
    udpClient.init(WIFI_SSID,WIFI_PASSWORD, UDP_PORT);
}

void loop() {
    const auto cmd = udpClient.get();
    for (int i = 0; i < ARRAY_LEN(strips); ++i) {
        if (cmd[i])
            strips[i].turnOn();
        else
            strips[i].turnOff();
    }
    yield();


}
