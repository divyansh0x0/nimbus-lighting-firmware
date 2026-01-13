//
// Created by divyansh on 1/13/26.
//
#pragma once

#include "FastLED.h"

namespace ROBO
{
    struct LEDStrip
    {
        /**
         * This is the CRGB array owned by LEDStrip. It will get auto deleted when the LEDStrip gets deleted;
         */
        CRGB *arr;
        int length;
        unsigned char id;

        void turnOn(const int count = -1, const int start = 0, const CRGB color = CRGB::White)
        {
            int end = count < 0 ? length : start+ count;
            for (int i = start; i < end; i++)
            {
                arr[i] = color;
            }
            FastLED.show();
        }

        void turnOff( const int count = -1, const int start = 0)
        {
            int end = count < 0 ? length : start + count;
            for (int i = start; i < end; i++)
            {
                arr[i] = CRGB::Black;
            }
        }
        ~LEDStrip(){
            delete [] arr;
        }
        operator CRGB*(){
            return this->arr;
        } 
        static LEDStrip create(int count, unsigned char id){
            CRGB* arr = (CRGB*) malloc(sizeof(CRGB) * count); 
            return LEDStrip{arr, count, id};
        }
    };

}
