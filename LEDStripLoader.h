#pragma once
#include <FastLED.h>
#include "LEDStrip.h"
// Ensure your config macros (LED_TYPE, COLOR_MODE) are visible here
// or pass them as template args. Assuming global macros for brevity:
#define LED_TYPE WS2812B
#define COLOR_MODE GBR
// 1. Recursive Declaration

namespace ROBO {
    template<int... PINS>
    struct LEDStripLoader;

    // 2. Recursive Step
    // Takes the First Pin, and a pack of Remaining Pins
    template<int FIRST_PIN, int... REST_PINS>
    struct LEDStripLoader<FIRST_PIN, REST_PINS...> {
        static void init(ROBO::LEDStrip *strips, int index = 0) {
            // Initialize the current strip (index) with the current pin (FIRST_PIN)
            FastLED.addLeds<LED_TYPE, FIRST_PIN, COLOR_MODE>(
                strips[index].getArr(),
                strips[index].getLength()
            );

            // Recurse: Call init for the remaining pins, incrementing the strip index
            LEDStripLoader<REST_PINS...>::init(strips, index + 1);
        }
    };

    // 3. Base Case (Stop Condition)
    // When there are no pins left, do nothing.
    template<>
    struct LEDStripLoader<> {
        static void init(ROBO::LEDStrip *strips, int index) {
            // No-op
        }
    };
}
