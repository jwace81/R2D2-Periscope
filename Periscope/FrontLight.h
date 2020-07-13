#ifndef FRONTLIGHT_H
#define FRONTLIGHT_H

#include "Common.h"
#include "FastLED.h"

enum FrontStates {
  FrontOff,
  FrontOn,
  FrontStrobe
};

void setFrontLightState(int mode, CRGB color = CRGB::White);

void processFrontLightCommand(char commandBuffer[], unsigned char commandLength);
void setupFrontLight();
void processFrontLight(unsigned long currentTime);

#endif
