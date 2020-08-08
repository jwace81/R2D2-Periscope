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
            fill_solid( this->leds, this->numLeds, this->color);
            break;

        case LIGHT_PANEL_STROBE:
            fill_solid( this->leds, this->numLeds, CRGB::Black);
            break;

        case LIGHT_PANEL_OFF:
        default:
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

  EVERY_N_MILLISECONDS(5)
  {
    fill_solid( this->leds, this->numLeds, lightState == 0 ? CRGB::Black : this->color);
    this->ledController->showLeds(brightness);
  }
}

void LightPanel::update(unsigned long currentTime) {
  switch(this->state) {
    case LIGHT_PANEL_STROBE:
      this->strobe();
      break;
  }
}
