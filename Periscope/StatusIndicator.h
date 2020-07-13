#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

#include "Common.h"
#include "FastLED.h"

enum StatusIndicatorStates {
  STATUS_INDICATOR_OFF,
  STATUS_INDICATOR_RANDOM,
  STATUS_INDICATOR_ALL_ON,
  STATUS_INDICATOR_SWEEP
};

void setStatusIndicatorState(int mode);

void processStatusIndicatorCommand(char commandBuffer[], unsigned char commandLength);
void setupStatusIndicator();
void processStatusIndicator(unsigned long currentTime);

#endif
