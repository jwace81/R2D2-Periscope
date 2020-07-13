#include "FrontLight.h"

CRGB frontLeds[NUM_FRONT_LEDS];
CRGB frontColor = CRGB::White;
CLEDController *frontController;
unsigned char frontBrightness = 32;
int currentFrontState = FrontStates::FrontOff;
unsigned long lastFrontUpdate;
bool frontStrobeState = false;

void setFrontLightState(int mode, CRGB color = CRGB::White) {
  switch(currentFrontState) {
    case FrontStates::FrontOn:
      Serial.println("Front On");
      fill_solid( &(frontLeds[0]), NUM_FRONT_LEDS, frontColor);
      break;

    case FrontStates::FrontStrobe:
      Serial.println("Front Strobe");
      fill_solid( &(frontLeds[0]), NUM_FRONT_LEDS, frontColor);
      frontStrobeState = true;
      break;

    case FrontStates::FrontOff:
    default:
      Serial.println("Front Off");
      fill_solid( &(frontLeds[0]), NUM_FRONT_LEDS, CRGB::Black);
      break;
  }

  frontController->showLeds(frontBrightness);
  lastFrontUpdate = millis();
}

void processFrontLightCommand(char commandBuffer[], unsigned char commandLength) {
  if (isDigit(commandBuffer[1])) {
    currentFrontState = commandBuffer[1] - '0';
  }

  if (commandLength > 2 && isDigit(commandBuffer[2])) {
    frontColor = colorMap[commandBuffer[2] - '0'];
  }
}

void setupFrontLight() {
  frontController = &FastLED.addLeds<NEOPIXEL, 11>(frontLeds, NUM_FRONT_LEDS);

  fill_solid( &(frontLeds[0]), NUM_FRONT_LEDS, CRGB::Black);
  currentFrontState = FrontStates::FrontOff;
}

void processFrontLight(unsigned long currentTime) {
  if (currentFrontState == FrontStates::FrontStrobe && currentTime - lastFrontUpdate > FRONT_STROBE_SPEED) {
    if (frontStrobeState) {
        fill_solid( &(frontLeds[0]), NUM_FRONT_LEDS, CRGB::Black);
    } else {
        fill_solid( &(frontLeds[0]), NUM_FRONT_LEDS, frontColor);
    }

    frontStrobeState = !frontStrobeState;
    frontController->showLeds(frontBrightness);
    lastFrontUpdate = currentTime;
  }
}
