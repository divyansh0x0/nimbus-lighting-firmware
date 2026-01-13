#include <Arduino.h>

#include "FastLED.h"
#include "LEDStrip.h"
#include "config.h"
#include "UDPClient.h"

#define LED_TYPE WS2812B
#define COLOR_MODE GBR
#define ARRAY_LEN(x) (int) sizeof(x) / (int)sizeof(x[0])
#define ID_START 0
#define LED_COUNT 10
#define TESTING_STRIP_PARTS 3

unsigned char id_count = BASE_ID_COUNT;

unsigned char getStripId() {
    return id_count++;
};
ROBO::LEDStrip strips[] = {
    ROBO::LEDStrip::create(LED_COUNT, getStripId()),
    ROBO::LEDStrip::create(LED_COUNT, getStripId()),
    ROBO::LEDStrip::create(LED_COUNT, getStripId()),
};
auto udpClient = ROBO::UDPClient();

void setup() {
    Serial.begin(115200);
    delay(250);
    pinMode(LED_BUILTIN, OUTPUT);

    FastLED.addLeds<WS2812B, 4, COLOR_MODE>(strips[0].getArr(), strips[0].getLength());
    FastLED.addLeds<WS2812B, 5, COLOR_MODE>(strips[1].getArr(), strips[1].getLength());

    FastLED.setBrightness(255);
    strips[0].turnOff();
    strips[1].turnOff();

    FastLED.show();

    udpClient.init(WIFI_SSID,WIFI_PASSWORD, UDP_PORT);
}

/**
 * For testing purpose only, redundant in production
*/
void singleStripCmd(const ROBO::LEDControlCommand cmd) {
    auto &led_strip = strips[0]; // use reference to avoid copying
    if (cmd.id < 0 || cmd.id >= TESTING_STRIP_PARTS) {
        Serial.println("id:" + String(cmd.id) + " is invalid. Skipping over");
        return;
    }

    int partSize = led_strip.getLength() / TESTING_STRIP_PARTS;
    int start = cmd.id * partSize;
    int end = start + partSize;
    if (cmd.id == TESTING_STRIP_PARTS - 1) {
        end = led_strip.getLength(); // include any remainder
    }
    int length = end - start;
    switch (cmd.state) {
        case ROBO::LEDState::ON:
            led_strip.turnOn(length, start);
            break;
        case ROBO::LEDState::OFF:
            led_strip.turnOff(length, start);
            break;
        case ROBO::LEDState::UNSET:
            // do nothing
            break;
    }
}


void handleMultiStripCmd(const ROBO::LEDControlCommand cmd) {
    if (cmd.id < BASE_ID_COUNT || cmd.id > BASE_ID_COUNT + ARRAY_LEN(strips)) {
        Serial.println("id:" + String(cmd.id) + " is invalid. Skipping over");
        return;
    }
    const int index = cmd.id - BASE_ID_COUNT;
    ROBO::LEDStrip strip = strips[index];
    switch (cmd.state) {
        case ROBO::LEDState::ON:
            strip.turnOn();
            break;
        case ROBO::LEDState::OFF:
            strip.turnOff();
            break;
        case ROBO::LEDState::UNSET:
            break;
    }
}

void loop() {
    const ROBO::LEDControlCommand cmd = udpClient.get();
    if (cmd.state == ROBO::LEDState::UNSET) {
        return;
    }
    Serial.println("----");
    Serial.println("id:" + String(cmd.id));
    Serial.println(cmd.state == ROBO::LEDState::ON ? "ON" : "OFF");
    singleStripCmd(cmd);
    delay(100);
}
