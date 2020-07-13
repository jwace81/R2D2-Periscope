#include "Walls.h"

CRGB wallLeds[2][NUM_WALL_LEDS];
CRGB wallColor = CRGB::Red;
CLEDController *wallControllers[2];
unsigned char wallBrightness = 64;
int currentWallState = WallStates::WallOff;
unsigned long lastWallUpdate;

void setupWalls() {
  wallControllers[0] = &FastLED.addLeds<NEOPIXEL, 9>(wallLeds[0], NUM_WALL_LEDS);
  wallControllers[1] = &FastLED.addLeds<NEOPIXEL, 10>(wallLeds[1], NUM_WALL_LEDS);

  setWallState(WallOff, wallColor);
}

void processWallCommand(char commandBuffer[], unsigned char commandLength) {
  bool stateChanged = false;
  if (isDigit(commandBuffer[1])) {
    currentWallState = commandBuffer[1] - '0';
    stateChanged = true;
  }

  if (commandLength > 2 && isDigit(commandBuffer[2])) {
    wallColor = colorMap[commandBuffer[2] - '0'];
    stateChanged = true;
  }
  
  if (stateChanged) {
    setWallState(currentWallState, wallColor);
  }
}

void setWallState(int mode, CRGB color = CRGB::Red) {
  wallColor = color;
  
  switch(currentWallState) {
    case WallStates::WallOn:
      Serial.println("Wall On");
      fill_solid( &(wallLeds[0][0]), NUM_WALL_LEDS, wallColor);
      fill_solid( &(wallLeds[1][0]), NUM_WALL_LEDS, wallColor);
      break;

    case WallStates::WallOff:
    default:
      Serial.println("Wall Off");
      fill_solid( &(wallLeds[0][0]), NUM_WALL_LEDS, CRGB::Black);
      fill_solid( &(wallLeds[1][0]), NUM_WALL_LEDS, CRGB::Black);
      break;
  }

  wallControllers[0]->showLeds(wallBrightness);
  wallControllers[1]->showLeds(wallBrightness);
  lastWallUpdate = millis();
}


void processWalls(unsigned long currentTime) {
}
