#pragma once
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "LEDStrip.h"
#include "ControlCommands.h"

inline void blink() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
}

namespace ROBO {
    class UDPClient {
    private:
        WiFiUDP udp;

    public:
        UDPClient() = default;

        void init(const char *ssid, const char *password, const unsigned int port) {
            WiFi.begin(ssid, password);
            Serial.println("Connecting to WiFi");
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                blink();
                Serial.println("Connecting to "+String(ssid) + "...");
            }
            Serial.println("Connected");
            Serial.print("Local IP:");
            Serial.println(WiFi.localIP());
            Serial.printf("Listening on UDP port %d\n", port);
            udp.begin(port);
        }

        LEDControlCommand get() {
            const int packetSize = udp.parsePacket();
            if (packetSize <= 0)
                return NO_LED_STATE_CHANGE;

            constexpr int BUFFER_LEN = 2;
            Serial.println(packetSize);
            if (packetSize != BUFFER_LEN) {
                // Drain invalid packet
                while (udp.available())
                    udp.read();
                return NO_LED_STATE_CHANGE;
            }

            uint8_t buf[BUFFER_LEN];
            udp.read(buf, BUFFER_LEN);

            // Cast buff[0] to 16 bit then shift 8 bits to left which give xxxxxxx00000000,
            // taking or with this gives xxxxxxxxyyyyyyyy
            // This assumes big endian of buff data;
            const uint8_t id = buf[0];
            const uint8_t state = buf[1];

            // Basic validation
            if (state > 1)
                return NO_LED_STATE_CHANGE;
            return LEDControlCommand(state, id);
        }
    };
}