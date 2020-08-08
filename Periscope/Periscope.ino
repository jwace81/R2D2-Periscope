#include "Common.h"
#include "Signal.h"
#include "Walls.h"
#include "FrontLight.h"
#include "BottomLights.h"

#include "FastLED.h"
#include "Wire.h"

#define I2CADDRESS 0x20  // 25 in Hexadecmal

char commandBuffer[MAX_COMMAND_LENGTH];
String commandString = "";
volatile boolean commandComplete = false;
unsigned long ledtimer = 0;      

void setup() {
  Serial.begin(9600);
  commandString.reserve(MAX_COMMAND_LENGTH);

  Wire.begin(I2CADDRESS);
  Wire.onReceive(i2cEvent);

  setupSignal();
  setupWalls();
  setupFrontLight();
  setupBottomLights();

  FastLED.show();
}

void loop() {
  unsigned long currentTime = millis();

  statusLEDCheck();
  
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
        processSignalCommand(commandBuffer, commandLength);
        break;
      
      case 'W':
        processWallCommand(commandBuffer, commandLength);
        break;

      case 'F':
        processFrontLightCommand(commandBuffer, commandLength);
        break;

      case 'I':
        processBottomLightsCommand(commandBuffer, commandLength);
        break;

      case 'X':
        setSignalState(SIGNAL_OFF);
        setWallState(WallOff);
        setFrontLightState(FrontOff);
        setBottomLightsState(BOTTOM_LIGHTS_OFF);
        break;
    }
  }

  processSignal(currentTime);
  processWalls(currentTime);
  processFrontLight(currentTime);
  processBottomLights(currentTime);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                               /////
/////                            Onboard LED Communication Status Trigger                           /////
/////                                                                                               /////
///// statusLEDOn - Turn on the LED                                                                 /////
///// statusLEDCheck - Check if enough time has passed and we should turn off the LED               /////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////// 
void statusLEDOn() {
  //digitalWrite(STATUSLEDPIN, HIGH);
  ledtimer = millis();
}

void statusLEDCheck() {
  //if(millis() - ledtimer >= STATUSLEDMSECS)
    //digitalWrite(STATUSLEDPIN, LOW);
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
    
    statusLEDOn();
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
  statusLEDOn();
}
