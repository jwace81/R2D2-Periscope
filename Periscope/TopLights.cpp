#include "TopLights.h"
#include "AnimationCommon.h"

CRGB leds[NUM_TOP_LIGHTS_LEDS];
CLEDController *controller;

CRGB color = TOP_LIGHTS_DEFAULT_COLOR;
int currentState = TOP_LIGHTS_DEFAULT_STATE;
unsigned char brightness = TOP_LIGHTS_DEFAULT_BRIGHTNESS;
uint8_t topLightsSpeed = 5;

unsigned long lastUpdate;

int chaseLed = 0;
int chaseDir = 1;

void setupTopLights() {
  controller = &FastLED.addLeds<NEOPIXEL, TOP_LIGHTS_PIN>(leds, NUM_TOP_LIGHTS_LEDS);
  fill_solid( &(leds[0]), NUM_TOP_LIGHTS_LEDS, TOP_LIGHTS_DEFAULT_OFF_COLOR);
}

void processTopLightsCommand(char commandBuffer[], unsigned char commandLength) {
  bool stateChanged = false;
  if (commandLength > 1 && isDigit(commandBuffer[1])) {
    currentState = commandBuffer[1] - '0';
    stateChanged = true;
  }

  if (commandLength > 2 && isDigit(commandBuffer[2])) {
    color = colorMap[commandBuffer[2] - '0'];
    stateChanged = true;
  }

  if (commandLength > 3 && isDigit(commandBuffer[3])) {
    topLightsSpeed = commandBuffer[3] - '0';
    stateChanged = true;
  }
  
  if (stateChanged) {
    setTopLightsState(currentState, color);
  }
}

void setTopLightsState(int mode, CRGB color = TOP_LIGHTS_DEFAULT_COLOR) {
  switch(currentState) {
    case TOP_LIGHTS_ON:
      Serial.println("Signal On");
      fill_solid( &(leds[0]), NUM_TOP_LIGHTS_LEDS, color);
      break;

    case TOP_LIGHTS_CHASE:
      Serial.println("Signal Chase");
      fill_solid( &(leds[0]), NUM_TOP_LIGHTS_LEDS, TOP_LIGHTS_DEFAULT_OFF_COLOR);
      chaseLed = -1;
      break;

    case TOP_LIGHTS_PULSE:
      Serial.println("Signal Pulse");
      fill_solid( &(leds[0]), NUM_TOP_LIGHTS_LEDS, color);
      break;

    case TOP_LIGHTS_PULSE2:
      Serial.println("Signal Pulse2");
      fill_solid( &(leds[0]), NUM_TOP_LIGHTS_LEDS, TOP_LIGHTS_DEFAULT_OFF_COLOR);
      break;

    case TOP_LIGHTS_RAINBOW:
      Serial.println("Signal Rainbow");
      fill_rainbow(&(leds[0]), NUM_TOP_LIGHTS_LEDS, 0, 255 / NUM_TOP_LIGHTS_LEDS);
      break;

    case TOP_LIGHTS_OFF:
    default:
      currentState = TOP_LIGHTS_OFF;
      Serial.println("Signal Off");
      fill_solid( &(leds[0]), NUM_TOP_LIGHTS_LEDS, TOP_LIGHTS_DEFAULT_OFF_COLOR);
      break;
  }

  controller->showLeds(brightness);
  lastUpdate = millis();
}

void chase() {
  uint8_t bpm = map(topLightsSpeed, 0, 9, 10, 60);
  chaseLed = beatquadwave8(bpm, 0, NUM_TOP_LIGHTS_LEDS - 1);

  EVERY_N_MILLISECONDS(5)
  {
    fadeToBlackBy(leds, NUM_TOP_LIGHTS_LEDS, 20);
    leds[chaseLed] = color;
    controller->showLeds(brightness);
  }
}

void pulse2() {
  uint8_t bpm = map(topLightsSpeed, 0, 9, 10, 60);
  uint8_t val = beatquadwave8(bpm, 0, 4);

  fill_solid( &(leds[0]), NUM_TOP_LIGHTS_LEDS, TOP_LIGHTS_DEFAULT_OFF_COLOR);

  if (val > 0) {
    int start = NUM_TOP_LIGHTS_LEDS / 2 - val + 1;
    int count = (val - 1) * 2 + 1;
    fill_solid( &(leds[NUM_TOP_LIGHTS_LEDS / 2 - val + 1]), (val - 1) * 2 + 1, color);
  }

  controller->showLeds(brightness);
}

void pulse() {
  uint8_t bpm = map(topLightsSpeed, 0, 9, 10, 30);
  unsigned char tempBrightness = beatquadwave8(bpm);
  
  controller->showLeds(tempBrightness);
}

void processTopLights(unsigned long currentTime) {
  switch(currentState) {
    case TOP_LIGHTS_CHASE:
      chase();
      break;

    case TOP_LIGHTS_PULSE2:
      pulse2();
      break;

    case TOP_LIGHTS_PULSE:
      pulse();
      break;
  }
}
