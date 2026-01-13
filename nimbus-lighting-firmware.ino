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

ROBO::LEDStrip strip1 = ROBO::LEDStrip::create(LED_COUNT, ID_START);
ROBO::LEDStrip strip2 = ROBO::LEDStrip::create(LED_COUNT, ID_START + 1);
ROBO::LEDStrip strip3 = ROBO::LEDStrip::create(LED_COUNT, ID_START + 2);
ROBO::UDPClient udpClient = ROBO::UDPClient();

void setup() {
    // write your initialization code here
    pinMode(LED_BUILTIN, OUTPUT);
    FastLED.addLeds<WS2812B, 1, COLOR_MODE>(strip1, strip1.length);
    FastLED.addLeds<WS2812B, 2, COLOR_MODE>(strip2, strip2.length);
    FastLED.setBrightness(255);
    strip1.turnOff();
    strip2.turnOff();

    FastLED.show();

    udpClient.init(WIFI_SSID,WIFI_PASSWORD, UDP_PORT);
}

void processByte() {
}

void loop() {
    const ROBO::ControlCommand cmd = udpClient.get();
    if (cmd.state == ROBO::LEDState::UNSET) {
        Serial.println("Invalid state of LED skipping");
        return;
    }
    Serial.println("----");
    Serial.println("id:" + String(cmd.id));
    Serial.println("time:"  + String(cmd.time) + "s");
    Serial.println(cmd.state == ROBO::LEDState::ON ? "ON" : "OFF");

    // strip1.turnOn(LED_COUNT/2);
    // delay(1000);
    // strip1.turnOff();
    // strip1.turnOn(LED_COUNT/2, LED_COUNT/2);
    // delay(2000);
}
