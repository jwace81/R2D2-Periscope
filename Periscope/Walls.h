#ifndef WALLS_H
#define WALLS_H

#include "Common.h"
#include "FastLED.h"

enum WallStates {
  WallOff,
  WallOn
};

void setWallState(int mode, CRGB color = CRGB::Red);

void processWallCommand(char commandBuffer[], unsigned char commandLength);
void setupWalls();
void processWalls(unsigned long currentTime);

#endif
