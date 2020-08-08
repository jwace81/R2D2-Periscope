#include "BottomLights.h"

#define BOTTOM_LEDS 12

CRGB bottomLeds[BOTTOM_LEDS];
CRGB bottomColor = CRGB::Red;
CLEDController *bottomController;
unsigned char bottomBrightness = 64;
int currentBottomState = BottomStates::BOTTOM_LIGHTS_OFF;
unsigned long lastBottomUpdate;
int sweepLed = 0;

void setupBottomLights() {
  bottomController = &FastLED.addLeds<NEOPIXEL, 10>(bottomLeds, BOTTOM_LEDS);

  setBottomLightsState(BottomStates::BOTTOM_LIGHTS_OFF);
}

void setBottomLightsState(int mode, CRGB color) {
  currentBottomState = mode;
  bottomColor = color;
  
  switch(currentBottomState) {
    case BOTTOM_LIGHTS_RANDOM:
      Serial.println("Status Indicator Random");
      break;

    case BOTTOM_LIGHTS_ALL_ON:
      {
        Serial.println("Status Indicator All On");
        fill_solid( &(bottomLeds[0]), BOTTOM_LEDS, bottomColor);
      }
      break;

    case BOTTOM_LIGHTS_SWEEP:
      Serial.println("Status Indicator Sweep");
      break;
  
    case BOTTOM_LIGHTS_OFF:
    default:
      {
        Serial.println("Bottom Lights Off");
        fill_solid( &(bottomLeds[0]), BOTTOM_LEDS, CRGB::Black);
      }
      break;
  }

  lastBottomUpdate = millis();
  bottomController->showLeds(bottomBrightness);
}

void processBottomLightsCommand(char commandBuffer[], unsigned char commandLength) {
  bool stateChanged = false;
  if (commandLength > 1 && isDigit(commandBuffer[1])) {
    currentBottomState = commandBuffer[1] - '0';
    stateChanged = true;
  }
  
  if (stateChanged) {
    setBottomLightsState(currentBottomState);
  }
}

uint8_t beatquadwave8_2( accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255,
                             uint32_t timebase = 0, uint8_t phase_offset = 0)
{
  uint8_t beat = beat8( beats_per_minute, timebase);
  uint8_t beatsin = quadwave8( beat + phase_offset);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8( beatsin, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}

void chase_2() {
  uint8_t bpm = map(20, 0, 9, 10, 60);
  sweepLed = beatquadwave8_2(bpm, 0, BOTTOM_LEDS - 1);

  EVERY_N_MILLISECONDS(5)
  {
    fadeToBlackBy(bottomLeds, BOTTOM_LEDS, 5);
    bottomLeds[sweepLed] = bottomColor;
    bottomController->showLeds(bottomBrightness);
  }
}

void processBottomLights(unsigned long currentTime) {
  if (currentBottomState == BOTTOM_LIGHTS_RANDOM && currentTime - lastBottomUpdate > 100) {
    fill_solid( &(bottomLeds[0]), BOTTOM_LEDS, CRGB::Black);
    bottomLeds[random(BOTTOM_LEDS)] = bottomColor;
    lastBottomUpdate = currentTime;
  
    bottomController->showLeds(bottomBrightness);
  }

  switch(currentBottomState) {
    case BOTTOM_LIGHTS_SWEEP:
      chase_2();
      break;
  }
}
