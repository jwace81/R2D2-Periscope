#ifndef TOP_LIGHTS_H
#define TOP_LIGHTS_H

#include "Common.h"
#include "FastLED.h"

enum TopLightsMode {
  TOP_LIGHTS_OFF,
  TOP_LIGHTS_ON,
  TOP_LIGHTS_CHASE,
  TOP_LIGHTS_PULSE,
  TOP_LIGHTS_PULSE2,
  TOP_LIGHTS_RAINBOW
};

#define TOP_LIGHTS_PIN 12
#define NUM_TOP_LIGHTS_LEDS 7
#define TOP_LIGHTS_CHASE_SPEED 75
#define TOP_LIGHTS_DEFAULT_BRIGHTNESS 128
#define TOP_LIGHTS_DEFAULT_STATE TOP_LIGHTS_OFF
#define TOP_LIGHTS_DEFAULT_COLOR CRGB::Blue
#define TOP_LIGHTS_DEFAULT_OFF_COLOR CRGB::Black

void setTopLightsState(int mode, CRGB color = TOP_LIGHTS_DEFAULT_COLOR);

void processTopLightsCommand(char commandBuffer[], unsigned char commandLength);
void setupTopLights();
void processTopLights(unsigned long currentTime);

#endif
