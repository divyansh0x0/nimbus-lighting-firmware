//
// Created by divyansh on 1/13/26.
//
#pragma once
namespace ROBO {
    class LEDStrip {
    private:
        unsigned char out_pin;
        unsigned char state = 0;


    public:
        /**
         * @param digital_out_pin
         */
        explicit LEDStrip(unsigned char digital_out_pin):out_pin(digital_out_pin) {
           pinMode(out_pin, OUTPUT);
        };
        void turnOn() {
            // Serial.println("Turning on LED connected to "+String(out_pin));
            analogWrite(out_pin, 255);
        }

        void turnOff() {
            // Serial.println("Turning off LED connected to "+String(out_pin));
            analogWrite(out_pin, 0);
        }
    };
}
