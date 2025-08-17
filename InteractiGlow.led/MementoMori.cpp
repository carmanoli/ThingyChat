#include "MementoMori.h"

// Egg emoji pattern (8x8)


static const CRGB eggPattern[8][8] = {
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(205, 133, 63), CRGB(205, 133, 63), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(205, 133, 63), CRGB(244, 164, 96), CRGB(244, 164, 96), CRGB(205, 133, 63), CRGB(0, 0, 0), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(244, 164, 96), CRGB(244, 164, 96), CRGB(244, 164, 96), CRGB(244, 164, 96), CRGB(0, 0, 0), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(250, 250, 210), CRGB(250, 250, 210), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(244, 164, 96), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(250, 250, 210), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(244, 164, 96), CRGB(244, 164, 96), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(244, 164, 96), CRGB(244, 164, 96), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(244, 164, 96), CRGB(255, 228, 181), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 228, 181), CRGB(255, 228, 181), CRGB(244, 164, 96), CRGB(244, 164, 96), CRGB(0, 0, 0), CRGB(0, 0, 0) }
};



static const CRGB heartPattern[8][8] = {
  { CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0) },
  { CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0) },
  { CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0) },
  { CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0) },
  { CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0) },
  { CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) }
};




static const CRGB skullPattern[8][8] = {
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(0, 0, 0) },
  { CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255) },
  { CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255) },
  { CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255) },
  { CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255) },
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0) },
  { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0) }
};



void MementMori::drawEgg(CRGB* leds, int matrixIndex, int numLeds) {
    drawEmoji(leds, matrixIndex, numLeds, eggPattern);
}

void MementMori::drawHeart(CRGB* leds, int matrixIndex, int numLeds) {
    drawEmoji(leds, matrixIndex, numLeds, heartPattern);
}

void MementMori::drawSkull(CRGB* leds, int matrixIndex, int numLeds) {
    drawEmoji(leds, matrixIndex, numLeds, skullPattern);
}

void MementMori::drawEmoji(CRGB* leds, int matrixIndex, int numLeds, const CRGB emojiPattern[8][8]) {
    int startLed = matrixIndex * 64;
    
    if (startLed + 63 >= numLeds || matrixIndex < 0) {
        return;
    }

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int ledIndex = startLed + (y * 8 + x);
            leds[ledIndex] = emojiPattern[y][x];
        }
    }
}