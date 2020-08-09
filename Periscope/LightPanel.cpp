#include "LightPanel.h"
#include "AnimationCommon.h"

LightPanel::LightPanel(CLEDController *ledController, CRGB *leds, int numLeds, CRGB color) {
  this->numLeds = numLeds;
  this->color = color;
  this->ledController = ledController;
  this->leds = leds;
  fill_solid( this->leds, this->numLeds, CRGB::Black);
  this->ledController->showLeds(this->brightness);
  this->state = LIGHT_PANEL_OFF;
}

void LightPanel::processCommand(char commandBuffer[], unsigned char commandLength) {
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
        case LIGHT_PANEL_ON:
            Serial.println("Panel On");
            fill_solid( this->leds, this->numLeds, this->color);
            break;

        case LIGHT_PANEL_STROBE:
            Serial.println("Panel Strobe");
            fill_solid( this->leds, this->numLeds, CRGB::Black);
            break;

        case LIGHT_PANEL_PULSE:
            Serial.println("Panel Pulse");
            fill_solid(this->leds, this->numLeds, this->color);
            break;

        case LIGHT_PANEL_OFF:
        default:
            Serial.println("Panel Strobe");
            fill_solid( this->leds, this->numLeds, CRGB::Black);
            break;
    }

    this->ledController->showLeds(this->brightness);
    this->lastUpdate = millis();
  }
}

void LightPanel::strobe() {
  uint8_t bpm = map(this->speed, 0, 9, 60, 480);
  uint8_t lightState = beatquadwave8(bpm, 0, 1);

  fill_solid( this->leds, this->numLeds, lightState == 0 ? CRGB::Black : this->color);
  this->ledController->showLeds(brightness);
}

void LightPanel::pulse() {
  uint8_t bpm = map(this->speed, 0, 9, 10, 30);
  unsigned char tempBrightness = beatquadwave8(bpm);
  
  this->ledController->showLeds(tempBrightness);
}

void LightPanel::update(unsigned long currentTime) {
  switch(this->state) {
    case LIGHT_PANEL_STROBE:
      this->strobe();
      break;

    case LIGHT_PANEL_PULSE:
      this->pulse();
      break;
  }
}

void LightPanel::stop()
{
  this->state = LIGHT_PANEL_OFF;
  fill_solid( this->leds, this->numLeds, CRGB::Black);
  this->ledController->showLeds(this->brightness);
}
