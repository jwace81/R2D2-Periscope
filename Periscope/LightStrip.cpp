#include "LightStrip.h"
#include "AnimationCommon.h"

LightStrip::LightStrip(CLEDController *ledController, CRGB *leds, int numLeds, CRGB color) {
  this->numLeds = numLeds;
  this->color = color;
  this->ledController = ledController;
  this->leds = leds;
  fill_solid( this->leds, this->numLeds, CRGB::Black);
  this->ledController->showLeds(this->brightness);
  this->state = LIGHT_STRIP_OFF;
}

void LightStrip::processCommand(char commandBuffer[], unsigned char commandLength) {
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
      case LIGHT_STRIP_ON:
        Serial.println("Strip On");
        fill_solid(this->leds, this->numLeds, this->color);
        break;

      case LIGHT_STRIP_CHASE:
        Serial.println("Strip Chase");
        fill_solid(this->leds, this->numLeds, CRGB::Black);
        break;

      case LIGHT_STRIP_PULSE:
        Serial.println("Strip Pulse");
        fill_solid(this->leds, this->numLeds, this->color);
        break;

      case LIGHT_STRIP_PULSE2:
        Serial.println("Strip Pulse 2");
        fill_solid(this->leds, this->numLeds, CRGB::Black);
        break;

      case LIGHT_STRIP_RAINBOW:
        Serial.println("Strip Rainbow");
        fill_rainbow(this->leds, this->numLeds, 0, 255 / this->numLeds);
        break;

      case LIGHT_STRIP_OFF:
      default:
        Serial.println("Strip Off");
        this->state = LIGHT_STRIP_OFF;
        fill_solid( leds, this->numLeds, CRGB::Black);
        break;
    }

    this->ledController->showLeds(this->brightness);
    this->lastUpdate = millis();
  }
}

void LightStrip::chase() {
  uint8_t bpm = map(this->speed, 0, 9, 10, 60);
  int chaseLed = beatquadwave8(bpm, 0, this->numLeds - 1);

  EVERY_N_MILLISECONDS(5)
  {
    fadeToBlackBy(this->leds, this->numLeds, 20);
    this->leds[chaseLed] = color;
    this->ledController->showLeds(this->brightness);
  }
}

void LightStrip::pulse() {
  uint8_t bpm = map(this->speed, 0, 9, 10, 30);
  unsigned char tempBrightness = beatquadwave8(bpm);
  
  this->ledController->showLeds(tempBrightness);
}

void LightStrip::pulse2() {
  uint8_t bpm = map(this->speed, 0, 9, 10, 60);
  uint8_t val = beatquadwave8(bpm, 0, 4);

  fill_solid( this->leds, this->numLeds, CRGB::Black);

  if (val > 0) {
    int start = this->numLeds / 2 - val + 1;
    int count = (val - 1) * 2 + 1;
    fill_solid( &(this->leds[this->numLeds / 2 - val + 1]), (val - 1) * 2 + 1, color);
  }

  this->ledController->showLeds(this->brightness);
}


void LightStrip::update(unsigned long currentTime) {
  switch(this->state) {
    case LIGHT_STRIP_CHASE:
      this->chase();
      break;

    case LIGHT_STRIP_PULSE2:
      this->pulse2();
      break;

    case LIGHT_STRIP_PULSE:
      this->pulse();
      break;
  }
}

void LightStrip::stop()
{
  this->state = LIGHT_STRIP_OFF;
  fill_solid( this->leds, this->numLeds, CRGB::Black);
  this->ledController->showLeds(this->brightness);
}
