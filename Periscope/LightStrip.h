#ifndef LIGHTSTRIP_H
#define LIGHTSTRIP_H

#include "Common.h"
#include "IAnimatorBase.h"
#include "FastLED.h"

enum LightStripStates {
  LIGHT_STRIP_OFF,
  LIGHT_STRIP_ON,
  LIGHT_STRIP_CHASE,
  LIGHT_STRIP_PULSE,
  LIGHT_STRIP_PULSE2,
  LIGHT_STRIP_RAINBOW
};

class LightStrip : public IAnimatorBase {
    protected:
        int numLeds;
        CRGB color;
        CLEDController *ledController;
        unsigned char brightness = 64;
        unsigned char state = LIGHT_STRIP_OFF;
        unsigned long lastUpdate;
        bool strobeState = false;
        char speed = 5;
        CRGB *leds;

  public:
    template <uint8_t DATA_PIN>
    static LightStrip *addLightStrip(int numLeds, CRGB color) {
      static CRGB *leds = new CRGB[numLeds];
      static CLEDController *ledController = &FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, numLeds);
      return new LightStrip(ledController, leds, numLeds, color);
    }

  virtual void processCommand(char commandBuffer[], unsigned char commandLength);
  virtual void update(unsigned long currentTime);
  virtual void stop();

  protected:
    LightStrip(CLEDController *ledController, CRGB *leds, int numLeds, CRGB color);

  private:
    void chase();
    void pulse();
    void pulse2();
};

#endif
