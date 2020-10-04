#ifndef COMMON_H
#define COMMON_H

#include "FastLED.h"

#define STATUSLEDMSECS 200                 // Amount of time to flash the led pin
#define STATUSLEDPIN 13                    // The builtin LED on the Arduino mini is on pin 13

#define MAX_COMMAND_LENGTH 10

#define FRONT_PIN 9
#define NUM_FRONT_LEDS 7
#define DEFAULT_FRONT_COLOR CRGB::White

#define LEFT_PIN 11
#define NUM_LEFT_LEDS 7
#define DEFAULT_LEFT_COLOR CRGB::Red

#define RIGHT_PIN 2
#define NUM_RIGHT_LEDS 7
#define DEFAULT_RIGHT_COLOR CRGB::Red

#define TOP_LIGHTS_PIN 12
#define NUM_TOP_LEDS 7
#define TOP_LIGHTS_CHASE_SPEED 75
#define DEFAULT_TOP_COLOR CRGB::Blue

#define BOTTOM_LIGHTS_PIN 10
#define NUM_BOTTOM_LEDS 12
#define DEFAULT_BOTTOM_COLOR CRGB::Red


const CRGB colorMap[] = {
  CRGB::Red,
  CRGB::Yellow,
  CRGB::Green,
  CRGB::Cyan,
  CRGB::Blue,
  CRGB::Magenta,
  CRGB::Orange,
  CRGB::Purple,
  CRGB::White,
  CRGB::Pink
};

#endif
