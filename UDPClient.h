#pragma once
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "LEDStrip.h"
#include "ControlCommands.h"

inline void blink() {
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
        static constexpr int state_count = 5;

    private:
        WiFiUDP udp;
        unsigned char last_state_byte = 0;
        unsigned char last_state_arr[state_count] = {0, 0, 0, 0, 0};
        unsigned char esp_id = 0;

    public:
        UDPClient(unsigned char esp_id)
            : esp_id(esp_id) {
        };

        void init(const char *ssid, const char *password, const unsigned int port) {
            WiFi.begin(ssid, password);
            Serial.println("Connecting to WiFi");
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                blink();
                Serial.println("Connecting to " + String(ssid) + "...");
            }
            Serial.println("Connected");
            Serial.print("Local IP:");
            Serial.println(WiFi.localIP());
            Serial.printf("Listening on UDP port %d\n", port);
            udp.begin(port);
        }

        unsigned char *get() {
            const int packetSize = udp.parsePacket();
            if (packetSize <= 0)
                return last_state_arr;
            constexpr int BUFFER_LEN = 2;
            uint8_t buf[BUFFER_LEN];
            udp.read(buf, BUFFER_LEN);

            // Skip if not the id of this ESP
            if (buf[0] != esp_id) {
                Serial.println("Skipping because " + String(buf[0]) + " != " + esp_id);
                return last_state_arr;
            }

            const uint8_t states = buf[1];
            if (states == last_state_byte) {
                Serial.println("Same state received: " + String(states, BIN));
                return last_state_arr;
            }
            unsigned char mask = 0b1;
            Serial.print(states, BIN);

            for (int i = 0; i < state_count; ++i) {
                int bit_index = 7 - i; // MSB first
                unsigned char new_state = (states >> bit_index) & 0x01;

                // Serial.println(
                    // "Updated state[" + String(i) + "] from " + String(last_state_arr[i]) + " to " + String(new_state));

                last_state_arr[i] = new_state;
            }

            last_state_byte = states;


            return last_state_arr;
        }
    };
}
