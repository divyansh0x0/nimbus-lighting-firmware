#include "Print.h"
#pragma once
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "LEDStrip.h"
#include "ControlCommands.h"

#define MAX_DATALINES 5
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
        LEDControlCommand cmd_array[MAX_DATALINES] = {};
        int id;
        void sendId(const IPAddress& serverIp, uint16_t serverPort) {
            uint8_t buf = static_cast<uint8_t>(id);
            udp.beginPacket(serverIp, serverPort);
            udp.write(&buf, 1);
            udp.endPacket();
        }

    public:
        UDPClient(int id): id(id){}

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
            // sendId(IPAddress(192,168,137,1), port);
        }

        LEDControlCommand* get() {
            const int packetSize = udp.parsePacket();
            if (packetSize <= 0)
                return cmd_array;

            constexpr int BUFFER_LEN = 1;
            if (packetSize != BUFFER_LEN) {
                // Drain invalid packet
                while (udp.available())
                    udp.read();
                return cmd_array;
            }

            uint8_t buf[BUFFER_LEN];
            udp.write(buf, BUFFER_LEN);
            Serial.print(buf[0], BIN);
            Serial.print("\n");
            unsigned char mask = 0b00010000;
            for (int i = 0; i < MAX_DATALINES; i++) {
                cmd_array[i].id = i;
                cmd_array[i].state = (mask & buf[0]) != 0 ? LEDState::ON : LEDState::OFF;
                mask >>= 1;
            }
            return cmd_array;
        }
    };
}