#pragma once
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "LEDStrip.h"
#include "ControlCommands.h"
//
// Created by divyansh on 1/13/26.
//

inline void blink() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
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
                blink();
                Serial.print(".");
            }
            Serial.println("Connected");
            Serial.print("Local IP:");
            Serial.println(WiFi.localIP());
            Serial.printf("Listening on UDP port %d\n", port);
            udp.begin(port);
        }

        ControlCommand get() {
            const int packetSize = udp.parsePacket();
            if (packetSize <= 0)
                return NO_LED_STATE_CHANGE;

            if (packetSize != 4) {
                // Drain invalid packet
                while (udp.available())
                    udp.read();
                return NO_LED_STATE_CHANGE;
            }

            uint8_t buf[4];
            udp.read(buf, 4);

            // Cast buff[0] to 16 bit then shift 8 bits to left which give xxxxxxx00000000,
            // taking or with this gives xxxxxxxxyyyyyyyy
            // This assumes big endian of buff data;
            const uint16_t timeSec = (static_cast<uint16_t>(buf[0]) << 8) | buf[1];
            const uint8_t id = buf[2];
            const uint8_t state = buf[3];

            // Basic validation
            if (state > 1)
                return NO_LED_STATE_CHANGE;
            return ControlCommand(state, id, timeSec);
        }
    };
}

// #include <ESP8266WiFi.h>
// #include <WiFiUdp.h>

// const char* ssid = "YOUR_WIFI_NAME";
// const char* password = "YOUR_WIFI_PASSWORD";

// WiFiUDP udp;
// unsigned int localUdpPort = 4210;   // Port ESP8266 will listen on
// char incomingPacket[255];           // Buffer for incoming packets

// void setup() {
//   Serial.begin(115200);

//   // Connect to WiFi
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("\nConnected");
//   Serial.print("ESP IP address: ");
//   Serial.println(WiFi.localIP());

//   // Start UDP
//   udp.begin(localUdpPort);
//   Serial.printf("Listening on UDP port %d\n", localUdpPort);
// }

// void loop() {
//   int packetSize = udp.parsePacket();
//   if (packetSize) {
//     // Read packet
//     int len = udp.read(incomingPacket, 254);
//     if (len > 0) {
//       incomingPacket[len] = 0;  // Null-terminate string
//     }

//     Serial.println("UDP Packet Received:");
//     Serial.printf("From %s:%d\n",
//                   udp.remoteIP().toString().c_str(),
//                   udp.remotePort());
//     Serial.printf("Data: %s\n", incomingPacket);

//     // Optional: send response back
//     udp.beginPacket(udp.remoteIP(), udp.remotePort());
//     udp.print("ACK from ESP8266");
//     udp.endPacket();
//   }
// }
