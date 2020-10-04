#include "BottomLights.h"
#include "AnimationCommon.h"

#define INDICATOR_SPEED 500

BottomLights::BottomLights(CLEDController *ledController, CRGB *leds, int numLeds, CRGB color) {
  this->numLeds = numLeds;
  this->color = color;
  this->ledController = ledController;
  this->leds = leds;
  fill_solid( this->leds, this->numLeds, CRGB::Black);
  this->ledController->showLeds(this->brightness);
  this->state = BOTTOM_LIGHTS_OFF;
}

void BottomLights::processCommand(char commandBuffer[], unsigned char commandLength) {
  bool stateChanged = false;
  if (commandLength > 1 && isDigit(commandBuffer[1])) {
    this->state = commandBuffer[1] - '0';
    stateChanged = true;
  }

  if (commandLength > 2 && isDigit(commandBuffer[2])) {
    this->color = colorMap[commandBuffer[2] - '0'];
    stateChanged = true;
  }
  
  if (commandLength > 3 && isDigit(commandBuffer[3])) {
    this->speed = commandBuffer[3] - '0';
    stateChanged = true;
  }

  if (stateChanged) {
    switch(this->state) {
      case BOTTOM_LIGHTS_ALL_ON:
        fill_solid(this->leds, this->numLeds, this->color);
        break;

      case BOTTOM_LIGHTS_RANDOM:
        fill_solid(this->leds, this->numLeds, CRGB::Black);
        break;

      case BOTTOM_LIGHTS_OFF:
      default:
        this->state = BOTTOM_LIGHTS_OFF;
        fill_solid( leds, this->numLeds, CRGB::Black);
        break;
    }

    this->ledController->showLeds(this->brightness);
    this->lastUpdate = millis();
  }
}

void BottomLights::update(unsigned long currentTime) {
  switch(this->state) {
    case BOTTOM_LIGHTS_RANDOM:
      this->randomLights(currentTime);
      break;
  }
}

void BottomLights::stop()
{
  this->state = BOTTOM_LIGHTS_OFF;
  fill_solid( this->leds, this->numLeds, CRGB::Black);
  this->ledController->showLeds(this->brightness);
}

void BottomLights::randomLights(unsigned long currentTime) {
  if (currentTime - this->lastUpdate > INDICATOR_SPEED) {
    fill_solid( &(this->leds[0]), this->numLeds, CRGB::Black);
    int ledIndex = random(this->numLeds);
    this->leds[ledIndex] = this->color;
    this->lastUpdate = currentTime;

    this->ledController->showLeds(this->brightness);
  }
}
