#include "radioactive.h"

static const byte radioactivePattern16x16[16][16] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0},
  {0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},
  {0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0},
  {0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

static uint32_t lastUpdateTime = 0;
static uint8_t brightnessPhase = 0;

void drawRadioactiveSymbol16x16(CRGB* leds, int numLeds, const int matrixIndices[4]) {
  CRGB backgroundColor = CRGB::Yellow;
  CRGB symbolColor = CRGB::Black;

  uint32_t currentTime = millis();
  if (currentTime - lastUpdateTime >= 8) {
    brightnessPhase++;
    lastUpdateTime = currentTime;
  }

  uint8_t brightness = sin8(brightnessPhase);
  CRGB scaledBackground = backgroundColor;
  scaledBackground.fadeToBlackBy(255 - brightness);

  for (int quadrant = 0; quadrant < 4; ++quadrant) {
    int matrixIndex = matrixIndices[quadrant];
    int startLed = matrixIndex * 64;

    if (startLed + 63 >= numLeds || matrixIndex < 0) {
      continue;
    }

    int patternOffsetY = (quadrant >= 2) ? 8 : 0;
    int patternOffsetX = (quadrant % 2 != 0) ? 8 : 0;

    for (int localY = 0; localY < 8; ++localY) {
      for (int localX = 0; localX < 8; ++localX) {
        int patternY = localY + patternOffsetY;
        int patternX = localX + patternOffsetX;
        int ledIndex = startLed + (localY * 8 + localX);

        if (radioactivePattern16x16[patternY][patternX] == 1) {
          leds[ledIndex] = symbolColor;
        } else {
          leds[ledIndex] = scaledBackground;
        }
      }
    }
  }
}