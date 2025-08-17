//emojis8.cpp
#include "Emojis8.h"

FASTLED_USING_NAMESPACE


// 8x8 patterns for emojis (1 = LED on, 0 = LED off)
const uint8_t heartPattern[8][8] = {
  {0, 1, 1, 0, 0, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}
};

const uint8_t crossPattern[8][8] = {
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1}
};

const uint8_t smilePattern[8][8] = {
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {0, 1, 1, 0, 0, 1, 1, 0}
};

// Function to draw an emoji on a specific matrix
void drawEmojiOnMatrix(CRGB leds[], int numLeds, int matrixIndex, const uint8_t pattern[8][8], CRGB color) {
  int startLED = matrixIndex * 64;
  
  if (startLED + 64 > numLeds) return;  // Safety check
  
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      int index = y * 8 + x;
      int ledIndex = startLED + index;
      leds[ledIndex] = (pattern[y][x] == 1) ? color : CRGB::Black;
    }
  }
  FastLED.show();
}

// Function to clear a specific emoji matrix
void clearEmojiMatrix(CRGB leds[], int numLeds, int matrixIndex) {
  int startLED = matrixIndex * 64;
  
  if (startLED + 64 > numLeds) return;  // Safety check
  
  for (int i = 0; i < 64; i++) {
    leds[startLED + i] = CRGB::Black;
  }
  FastLED.show();
}