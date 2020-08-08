#include "Common.h"
#include "TopLights.h"
#include "BottomLights.h"
#include "LightPanel.h";

#include "FastLED.h"
#include "Wire.h"

#define I2CADDRESS 0x20  // 25 in Hexadecmal

char commandBuffer[MAX_COMMAND_LENGTH];
String commandString = "";
volatile boolean commandComplete = false;
unsigned long ledtimer = 0;

CRGB frontleds[NUM_FRONT_LEDS];
CRGB leftleds[NUM_LEFT_LEDS];
CRGB rightleds[NUM_RIGHT_LEDS];

LightPanel *frontLights;
LightPanel *leftLights;
LightPanel *rightLights;

#define NUM_ANIMATORS 3

IAnimatorBase *animators[NUM_ANIMATORS];

void setup() {
  Serial.begin(9600);
  commandString.reserve(MAX_COMMAND_LENGTH);

  Wire.begin(I2CADDRESS);
  Wire.onReceive(i2cEvent);

  setupTopLights();
  setupBottomLights();

  frontLights = LightPanel::addLightPanel<FRONT_PIN>(frontleds, NUM_FRONT_LEDS, DEFAULT_FRONT_COLOR);
  leftLights = LightPanel::addLightPanel<LEFT_PIN>(leftleds, NUM_LEFT_LEDS, DEFAULT_LEFT_COLOR);
  rightLights = LightPanel::addLightPanel<RIGHT_PIN>(rightleds, NUM_RIGHT_LEDS, DEFAULT_RIGHT_COLOR);

  animators[0] = frontLights;
  animators[1] = leftLights;
  animators[2] = rightLights;

  Serial.println("Ready");
}

void loop() {
  unsigned long currentTime = millis();
  
  if(commandComplete) {
    commandString.trim();

    Serial.print(F("Received Command: "));
    Serial.println(commandString);

    int commandLength = commandString.length();
    commandString.toCharArray(commandBuffer, MAX_COMMAND_LENGTH);
    commandString="";
    commandComplete = false;

    switch(commandBuffer[0]) {
      case 'S':
        processTopLightsCommand(commandBuffer, commandLength);
        break;
      
      case 'W':
        leftLights->processCommand(commandBuffer, commandLength);
        rightLights->processCommand(commandBuffer, commandLength);
        break;
      
      case 'L':
        leftLights->processCommand(commandBuffer, commandLength);
        break;
      
      case 'R':
        rightLights->processCommand(commandBuffer, commandLength);
        break;

      case 'F':
        frontLights->processCommand(commandBuffer, commandLength);
        break;

      case 'I':
        processBottomLightsCommand(commandBuffer, commandLength);
        break;

      case 'X':
        setTopLightsState(TOP_LIGHTS_OFF);
        setBottomLightsState(BOTTOM_LIGHTS_OFF);
        break;
    }
  }

  processTopLights(currentTime);
  processBottomLights(currentTime);

  for (int i = 0; i < NUM_ANIMATORS; i++) {
    animators[i]->update(currentTime);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                               /////
/////                             Serial & I2C Communication Functions                              /////
/////                                                                                               /////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////  
    
      
/////////////////////////////////////////////////////////
///*****          Serial Event Function          *****///
/////////////////////////////////////////////////////////
/// This routine is run between loop() runs, so using ///
/// delay inside loop can delay response.  Multiple   ///
/// bytes of data may be available.                   ///
/////////////////////////////////////////////////////////
      
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\r' || inChar == '\n') {
      commandComplete = true;
    } else {
      commandString += inChar;
    }
  }
}

/////////////////////////////////////////////////////////
///*****            I2C Event Function           *****///
/////////////////////////////////////////////////////////
///  This routine is run when an onRecieve event is   ///
///     triggered.  Multiple bytes of data may be     ///
///                    available.                     ///
/////////////////////////////////////////////////////////            
      
void i2cEvent(int howMany)
{  
  commandString = "";
  int i=0;
  while(Wire.available()) {
    char inChar = (char)Wire.read();
    if(i<MAX_COMMAND_LENGTH) {
       commandString += inChar;
    }
    
    i++;
  }
  commandComplete = true;
}
