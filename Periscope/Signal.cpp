#include "Signal.h"

CRGB leds[NUM_SIGNAL_LEDS];
CLEDController *controller;

CRGB color = SIGNAL_DEFAULT_COLOR;
int currentState = SIGNAL_DEFAULT_STATE;
unsigned char brightness = SIGNAL_DEFAULT_BRIGHTNESS;
uint8_t signalSpeed = 5;

unsigned long lastUpdate;

int chaseLed = 0;
int chaseDir = 1;

void setupSignal() {
  controller = &FastLED.addLeds<NEOPIXEL, SIGNAL_PIN>(leds, NUM_SIGNAL_LEDS);
  fill_solid( &(leds[0]), NUM_SIGNAL_LEDS, SIGNAL_DEFAULT_OFF_COLOR);
}

void processSignalCommand(char commandBuffer[], unsigned char commandLength) {
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
    signalSpeed = commandBuffer[3] - '0';
    stateChanged = true;
  }
  
  if (stateChanged) {
    setSignalState(currentState, color);
  }
}

void setSignalState(int mode, CRGB color = SIGNAL_DEFAULT_COLOR) {
  switch(currentState) {
    case SIGNAL_ON:
      Serial.println("Signal On");
      fill_solid( &(leds[0]), NUM_SIGNAL_LEDS, color);
      break;

    case SIGNAL_CHASE:
      Serial.println("Signal Chase");
      fill_solid( &(leds[0]), NUM_SIGNAL_LEDS, SIGNAL_DEFAULT_OFF_COLOR);
      chaseLed = -1;
      break;

    case SIGNAL_PULSE:
      Serial.println("Signal Pulse");
      fill_solid( &(leds[0]), NUM_SIGNAL_LEDS, color);
      break;

    case SIGNAL_PULSE2:
      Serial.println("Signal Pulse2");
      fill_solid( &(leds[0]), NUM_SIGNAL_LEDS, SIGNAL_DEFAULT_OFF_COLOR);
      break;

    case SIGNAL_RAINBOW:
      Serial.println("Signal Rainbow");
      fill_rainbow(&(leds[0]), NUM_SIGNAL_LEDS, 0, 255 / NUM_SIGNAL_LEDS);
      break;

    case SIGNAL_OFF:
    default:
      currentState = SIGNAL_OFF;
      Serial.println("Signal Off");
      fill_solid( &(leds[0]), NUM_SIGNAL_LEDS, SIGNAL_DEFAULT_OFF_COLOR);
      break;
  }

  controller->showLeds(brightness);
  lastUpdate = millis();
}

uint8_t beatquadwave8( accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255,
                             uint32_t timebase = 0, uint8_t phase_offset = 0)
{
  uint8_t beat = beat8( beats_per_minute, timebase);
  uint8_t beatsin = quadwave8( beat + phase_offset);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8( beatsin, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}

void chase() {
  uint8_t bpm = map(signalSpeed, 0, 9, 10, 60);
  chaseLed = beatquadwave8(bpm, 0, NUM_SIGNAL_LEDS - 1);

  EVERY_N_MILLISECONDS(5)
  {
    fadeToBlackBy(leds, NUM_SIGNAL_LEDS, 20);
    leds[chaseLed] = color;
    controller->showLeds(brightness);
  }
}

void pulse2() {
  uint8_t bpm = map(signalSpeed, 0, 9, 10, 60);
  uint8_t val = beatquadwave8(bpm, 0, 4);

  fill_solid( &(leds[0]), NUM_SIGNAL_LEDS, SIGNAL_DEFAULT_OFF_COLOR);

  if (val > 0) {
    int start = NUM_SIGNAL_LEDS / 2 - val + 1;
    int count = (val - 1) * 2 + 1;
    fill_solid( &(leds[NUM_SIGNAL_LEDS / 2 - val + 1]), (val - 1) * 2 + 1, color);
  }

  controller->showLeds(brightness);
}

void pulse() {
  uint8_t bpm = map(signalSpeed, 0, 9, 10, 30);
  unsigned char tempBrightness = beatquadwave8(bpm);
  
  controller->showLeds(tempBrightness);
}

void processSignal(unsigned long currentTime) {
  switch(currentState) {
    case SIGNAL_CHASE:
      chase();
      break;

    case SIGNAL_PULSE2:
      pulse2();
      break;

    case SIGNAL_PULSE:
      pulse();
      break;
  }
}
