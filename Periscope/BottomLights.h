#ifndef BOTTOMLIGHTS_H
#define BOTTOMLIGHTS_H

#include "Common.h"
#include "IAnimatorBase.h"
#include "FastLED.h"

enum BottomLightStates {
  BOTTOM_LIGHTS_OFF,
  BOTTOM_LIGHTS_RANDOM,
  BOTTOM_LIGHTS_ALL_ON,
};

class BottomLights : public IAnimatorBase {
    protected:
        int numLeds;
        CRGB color;
        CLEDController *ledController;
        unsigned char brightness = 64;
        unsigned char state = BOTTOM_LIGHTS_OFF;
        unsigned long lastUpdate;
        bool strobeState = false;
        char speed = 5;
        CRGB *leds;

  public:
    template <uint8_t DATA_PIN>
    static BottomLights *initialize() {
      static CRGB *leds = new CRGB[NUM_BOTTOM_LEDS];
      static CLEDController *ledController = &FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_BOTTOM_LEDS);
      return new BottomLights(ledController, leds, NUM_BOTTOM_LEDS, DEFAULT_BOTTOM_COLOR);
    }

  virtual void processCommand(char commandBuffer[], unsigned char commandLength);
  virtual void update(unsigned long currentTime);
  virtual void stop();

  protected:
    BottomLights(CLEDController *ledController, CRGB *leds, int numLeds, CRGB color);

  private:
    void randomLights(unsigned long currentTime);
};

#endif
