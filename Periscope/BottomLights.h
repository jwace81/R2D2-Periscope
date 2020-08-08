#ifndef BOTTOMLIGHTS_H
#define BOTTOMLIGHTS_H

#include "Common.h"
#include "FastLED.h"

enum BottomStates {
  BOTTOM_LIGHTS_OFF,
  BOTTOM_LIGHTS_RANDOM,
  BOTTOM_LIGHTS_ALL_ON,
  BOTTOM_LIGHTS_SWEEP
};

void setBottomLightsState(int mode, CRGB color = CRGB::Red);

void processBottomLightsCommand(char commandBuffer[], unsigned char commandLength);
void setupBottomLights();
void processBottomLights(unsigned long currentTime);

#endif
