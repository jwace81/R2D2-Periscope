#ifndef LIGHTPANEL_H
#define LIGHTPANEL_H

#include "Common.h"
#include "IAnimatorBase.h"
#include "FastLED.h"

enum LightPanelStates {
  LIGHT_PANEL_OFF,
  LIGHT_PANEL_ON,
  LIGHT_PANEL_STROBE
};

class LightPanel : public IAnimatorBase {
    protected:
        int numLeds;
        CRGB color;
        CLEDController *ledController;
        unsigned char brightness = 64;
        unsigned char state = LIGHT_PANEL_OFF;
        unsigned long lastUpdate;
        bool strobeState = false;
        char speed = 5;
        CRGB *leds;

    public:
        template <uint8_t DATA_PIN>
        static LightPanel *addLightPanel(CRGB* leds, int numLeds, CRGB color)  {
          static CLEDController *ledController = &FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, numLeds);
          return new LightPanel(ledController, leds, numLeds, color);
        }

        void processCommand(char commandBuffer[], unsigned char commandLength);
        void update(unsigned long currentTime);

    protected:
        LightPanel(CLEDController *ledController, CRGB *leds, int numLeds, CRGB color);

    private:
        void strobe();
};

#endif
