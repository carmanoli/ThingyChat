#ifndef EMOJIS8_H
#define EMOJIS8_H

#include <Arduino.h>  // For String and Arduino core functions
#include <FastLED.h>  // For CRGB and FastLED functions
#include "Config.h"

FASTLED_USING_NAMESPACE

// Function prototypes

void drawEmojiOnMatrix(CRGB leds[], int numLeds, int matrixIndex, const uint8_t pattern[8][8], CRGB color);
void clearEmojiMatrix(CRGB leds[], int numLeds, int matrixIndex);

// Emoji patterns
extern const uint8_t heartPattern[8][8];
extern const uint8_t crossPattern[8][8];
extern const uint8_t smilePattern[8][8];

#endif