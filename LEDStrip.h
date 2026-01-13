//
// Created by divyansh on 1/13/26.
//
#pragma once

#include "FastLED.h"

namespace ROBO {
    class LEDStrip {
    private:
        CRGB *leds;
        unsigned int length;
        unsigned char id;

        LEDStrip(CRGB *leds, const int count, const unsigned char id) {
            this->leds = leds;
            this->length = count;
            this->id = id;
        };;

    public:
        /**
         * This is the CRGB array owned by LEDStrip. It will get auto deleted when the LEDStrip gets deleted;
         */


        LEDStrip() = delete;

        // Copy constructor
        LEDStrip(const LEDStrip &other)
            : length(other.length), id(other.id) {
            leds = new CRGB[length];
            for (size_t i = 0; i < length; ++i)
                leds[i] = other.leds[i];
        }

        // Copy assignment (deep copy)
        LEDStrip &operator=(const LEDStrip &other) {
            if (this == &other)
                return *this;

            delete[] leds;

            length = other.length;
            id = other.id;
            leds = new CRGB[length];

            for (size_t i = 0; i < length; ++i)
                leds[i] = other.leds[i];

            return *this;
        }

        unsigned int getLength() const {
            return this->length;
        }

        struct CRGB * getArr() const {
            return this->leds;
        }

        // Move constructor
        LEDStrip(LEDStrip &&other) noexcept
            : leds(other.leds), length(other.length), id(other.id) {
            other.leds = nullptr;
            other.length = 0;
        }

        // Move assignment
        LEDStrip &operator=(LEDStrip &&other) noexcept {
            if (this == &other)
                return *this;

            delete[] leds;

            leds = other.leds;
            length = other.length;
            id = other.id;

            other.leds = nullptr;
            other.length = 0;
            return *this;
        }
        void turnOn(const CRGB color = CRGB::White) {
            this->turnOn(-1, 0, color);
        }
        void turnOn(const int count, const int start = 0, const CRGB color = CRGB::White) {
            const int end = count < 0 ? length : start + count;
            if (start < 0 || end > length)
                return;
            for (int i = start; i < end; i++) {
                leds[i] = color;
            }
            Serial.println("id:" + String(id) + " is ON from " + String(start) + " to " + String(end));
            FastLED.show();
        }
        void turnOff() {
            this->turnOff(-1,0);
        }
        void turnOff(const int count, const int start = 0) {
            const int end = count < 0 ? length : start + count;
            if (start < 0 || end > length)
                return;

            for (int i = start; i < end; i++) {
                leds[i] = CRGB::Black;
            }
            Serial.println("id:" + String(id) + " is OFF from " + String(start) + " to " + String(end));
            FastLED.show();
        }

        ~LEDStrip() {
            delete [] leds;
        }

        static LEDStrip create(const int count, const unsigned char id) {
            return LEDStrip{new CRGB[count], count, id};
        }
    };
}