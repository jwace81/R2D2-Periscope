#include "Signal.h"

CRGB leds[NUM_SIGNAL_LEDS];
CLEDController *controller;

CRGB color = SIGNAL_DEFAULT_COLOR;
int currentState = SIGNAL_DEFAULT_STATE;
unsigned char brightness;

unsigned long lastUpdate;

int chaseLed = 0;
int chaseDir = 1;

void setupSignal() {
  controller = &FastLED.addLeds<APA106, SIGNAL_PIN>(leds, NUM_SIGNAL_LEDS);
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


void processSignal(unsigned long currentTime) {
  switch(currentState) {
    case SIGNAL_CHASE:
      {
        if (currentTime - lastUpdate > SIGNAL_CHASE_SPEED) {
          chaseLed += chaseDir;
          if (chaseLed >= NUM_SIGNAL_LEDS) {
            chaseDir = -chaseDir;
            chaseLed = NUM_SIGNAL_LEDS - 2;
          } else if (chaseLed < 0) {
            chaseDir = -chaseDir;
            chaseLed = 1;
          }
          
          leds[chaseLed] = color;
          controller->showLeds(brightness);
          leds[chaseLed] = SIGNAL_DEFAULT_OFF_COLOR;
          lastUpdate = currentTime;
        }
      }
      break;

    case SIGNAL_PULSE2:
      {
        unsigned char wave = quadwave8((currentTime - lastUpdate) / 10);
        unsigned char val = map8(wave, 0, 4);
  
        fill_solid( &(leds[0]), NUM_SIGNAL_LEDS, SIGNAL_DEFAULT_OFF_COLOR);
  
        if (val > 0) {
          int start = NUM_SIGNAL_LEDS / 2 - val + 1;
          int count = (val - 1) * 2 + 1;
          fill_solid( &(leds[NUM_SIGNAL_LEDS / 2 - val + 1]), (val - 1) * 2 + 1, color);
        }
  
        controller->showLeds(brightness);
      }
      break;

    case SIGNAL_PULSE:
      {
        unsigned char tempBrightness = map8(quadwave8((currentTime - lastUpdate) / 10), 0, brightness);
        
        controller->showLeds(tempBrightness);
      }
      break;
  }
}
