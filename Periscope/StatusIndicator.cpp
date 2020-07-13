#include "StatusIndicator.h"

#define STATUS_INDICATOR_LEDS 12

int indicatorPins[STATUS_INDICATOR_LEDS];
int indicatorPinState[STATUS_INDICATOR_LEDS];
unsigned long lastIndicatorUpdate;

int indicatorState = STATUS_INDICATOR_OFF;

int sweepPin = 0;

void setupStatusIndicator() {
  indicatorPins[0] = 3;
  indicatorPins[1] = 4;
  indicatorPins[2] = 5;
  indicatorPins[3] = 6;
  indicatorPins[4] = 7;
  indicatorPins[5] = 8;
  indicatorPins[6] = 12;
  indicatorPins[7] = A0;
  indicatorPins[8] = A1;
  indicatorPins[9] = A2;
  indicatorPins[10] = A3;
  indicatorPins[11] = 13;

  for(int i = 0; i < STATUS_INDICATOR_LEDS; i++) {
    indicatorPinState[i] = LOW;
    pinMode(indicatorPins[i], OUTPUT);
    digitalWrite(indicatorPins[i], indicatorPinState[i]);
  }
}

void setStatusIndicatorState(int mode) {
  switch(indicatorState) {
    case STATUS_INDICATOR_RANDOM:
      Serial.println("Status Indicator Random");
      break;

    case STATUS_INDICATOR_ALL_ON:
      {
        Serial.println("Status Indicator All On");
        for(int i = 0; i < STATUS_INDICATOR_LEDS; i++) {
          indicatorPinState[i] = HIGH;
          digitalWrite(indicatorPins[i], indicatorPinState[i]);
        }
      }
      break;

    case STATUS_INDICATOR_SWEEP:
      Serial.println("Status Indicator Sweep");
      break;
  
    case STATUS_INDICATOR_OFF:
    default:
      Serial.println("Status Indicator Off");
      for(int i = 0; i < STATUS_INDICATOR_LEDS; i++) {
        indicatorPinState[i] = LOW;
        digitalWrite(indicatorPins[i], indicatorPinState[i]);
      }
      break;
  }

  lastIndicatorUpdate = millis();
}

void processStatusIndicatorCommand(char commandBuffer[], unsigned char commandLength) {
  bool stateChanged = false;
  if (commandLength > 1 && isDigit(commandBuffer[1])) {
    indicatorState = commandBuffer[1] - '0';
    stateChanged = true;
  }
  
  if (stateChanged) {
    setStatusIndicatorState(indicatorState);
  }
}

void processStatusIndicator(unsigned long currentTime) {
  switch(indicatorState) {
    case STATUS_INDICATOR_RANDOM:
      {
        if (currentTime - lastIndicatorUpdate > INDICATOR_SPEED) {
          for(int i = 0; i < STATUS_INDICATOR_LEDS; i++) {
            indicatorPinState[i] = LOW;
          }
      
          indicatorPinState[random(STATUS_INDICATOR_LEDS)] = HIGH;
      
          for(int i = 0; i < STATUS_INDICATOR_LEDS; i++) {
            digitalWrite(indicatorPins[i], indicatorPinState[i]);
          }
      
          lastIndicatorUpdate = currentTime;
        }
      }
      break;

    case STATUS_INDICATOR_ALL_ON:
      {
        for(int i = 0; i < STATUS_INDICATOR_LEDS; i++) {
          indicatorPinState[i] = HIGH;
          digitalWrite(indicatorPins[i], indicatorPinState[i]);
        }
      }
      break;

    case STATUS_INDICATOR_SWEEP:
      {
        if (currentTime - lastIndicatorUpdate > INDICATOR_SPEED) {
          indicatorPinState[sweepPin] = LOW;
          digitalWrite(indicatorPins[sweepPin++], LOW);

          if (sweepPin >= STATUS_INDICATOR_LEDS) {
            sweepPin = 0;
          }
          
          indicatorPinState[sweepPin] = HIGH;
          digitalWrite(indicatorPins[sweepPin], HIGH);
          
          lastIndicatorUpdate = currentTime;
        }
      }
  }
}
