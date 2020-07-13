#ifndef COMMON_H
#define COMMON_H

#include "FastLED.h"

#define STATUSLEDMSECS 200                 // Amount of time to flash the led pin
#define STATUSLEDPIN 13                    // The builtin LED on the Arduino mini is on pin 13

#define MAX_COMMAND_LENGTH 10

#define INDICATOR_SPEED 500

#define FRONT_STROBE_SPEED 50

#define NUM_WALL_LEDS 7
#define NUM_FRONT_LEDS 7

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
