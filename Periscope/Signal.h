#ifndef SIGNAL_H
#define SIGNAL_H

#include "Common.h"
#include "FastLED.h"

enum SignalMode {
  SIGNAL_OFF,
  SIGNAL_ON,
  SIGNAL_CHASE,
  SIGNAL_PULSE,
  SIGNAL_PULSE2,
  SIGNAL_RAINBOW
};

#define SIGNAL_PIN 12
#define NUM_SIGNAL_LEDS 7
#define SIGNAL_CHASE_SPEED 75
#define SIGNAL_DEFAULT_BRIGHTNESS 128
#define SIGNAL_DEFAULT_STATE SIGNAL_OFF
#define SIGNAL_DEFAULT_COLOR CRGB::Blue
#define SIGNAL_DEFAULT_OFF_COLOR CRGB::Black

void setSignalState(int mode, CRGB color = SIGNAL_DEFAULT_COLOR);

void processSignalCommand(char commandBuffer[], unsigned char commandLength);
void setupSignal();
void processSignal(unsigned long currentTime);

#endif
