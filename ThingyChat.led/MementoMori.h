#ifndef MEMENTMORI_H
#define MEMENTMORI_H

#include <FastLED.h>

class MementMori {
public:
    static void drawEgg(CRGB* leds, int matrixIndex, int numLeds);
    static void drawHeart(CRGB* leds, int matrixIndex, int numLeds);
    static void drawSkull(CRGB* leds, int matrixIndex, int numLeds);
    
private:
    static void drawEmoji(CRGB* leds, int matrixIndex, int numLeds, const CRGB emojiPattern[8][8]);
};

#endif