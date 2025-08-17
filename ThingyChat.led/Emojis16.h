// File: Emojis16.h

#ifndef EMOJIS16_H
#define EMOJIS16_H

#include <FastLED.h>

// ... (existing function declaration for drawEmoji16x16) ...
void drawEmoji16x16(CRGB* leds, int numLeds, const int matrixIndices[4], const CRGB pattern[16][16]);

/**
 * @brief Draws a 16x16 emoji sequentially, pixel by pixel, with a delay.
 *        This is a "blocking" function; it will pause program execution until the animation is complete.
 * 
 * @param leds The pointer to the array of LEDs.
 * @param numLeds The total number of LEDs.
 * @param matrixIndices The 4 matrix indices for the 2x2 block.
 * @param pattern The 16x16 CRGB pattern to draw.
 * @param pixelDelay_ms The delay in milliseconds between drawing each pixel.
 */
void drawEmoji16x16_Animated(CRGB* leds, int numLeds, const int matrixIndices[4], const CRGB pattern[16][16], int pixelDelay_ms); // <<< NEW FUNCTION

// ... (existing printPatternAsBinary function) ...
void printPatternAsBinary(const CRGB pattern[16][16], const char* patternName);

// ... (existing extern const CRGB declarations) ...
extern const CRGB smileyFace16[16][16];

#endif // EMOJIS16_H