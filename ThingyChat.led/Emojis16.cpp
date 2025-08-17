// File: Emojis16.cpp
// Contains definitions for 16x16 emoji patterns and functions to display them.

#include "Emojis16.h"
#include <Arduino.h> // Required for Serial functions used in printPatternAsBinary

// --- Pattern Definitions ---
// Define all your 16x16 full-color emoji patterns here.

const CRGB smileyFace16[16][16] = {
  { CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold },
  { CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold },
  { CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold },
  { CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold },
  { CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Black, CRGB::Black }
};

// Add more 16x16 emoji definitions here in the future
// const CRGB anotherEmoji16[16][16] = { ... };


// --- Function Implementations ---

/**
 * @brief Instantly draws a 16x16 emoji on a 2x2 block of 8x8 matrices.
 */
void drawEmoji16x16(CRGB* leds, int numLeds, const int matrixIndices[4], const CRGB pattern[16][16]) {
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
        leds[ledIndex] = pattern[patternY][patternX];
      }
    }
  }
}

/**
 * @brief Draws a 16x16 emoji sequentially, pixel by pixel, with a delay. (Blocking)
 */
void drawEmoji16x16_Animated(CRGB* leds, int numLeds, const int matrixIndices[4], const CRGB pattern[16][16], int pixelDelay_ms) {
  for (int patternY = 0; patternY < 16; ++patternY) {
    for (int patternX = 0; patternX < 16; ++patternX) {
      int quadrant = 0;
      if (patternY < 8 && patternX >= 8) quadrant = 1;
      else if (patternY >= 8 && patternX < 8) quadrant = 2;
      else if (patternY >= 8 && patternX >= 8) quadrant = 3;

      int matrixIndex = matrixIndices[quadrant];

      if (matrixIndex < 0 || (matrixIndex * 64) >= numLeds) continue;

      int localX = patternX % 8;
      int localY = patternY % 8;
      int startLed = matrixIndex * 64;
      int ledIndex = startLed + (localY * 8 + localX);

      leds[ledIndex] = pattern[patternY][patternX];
      
      FastLED.show();
      delay(pixelDelay_ms);
    }
  }
}

/**
 * @brief Prints a 16x16 CRGB pattern to the Serial Monitor as a binary matrix.
 */
void printPatternAsBinary(const CRGB pattern[16][16], const char* patternName) {
  Serial.println();
  Serial.print("const byte ");
  Serial.print(patternName);
  Serial.println("[16][16] = {");

  for (int y = 0; y < 16; y++) {
    Serial.print("  {");
    for (int x = 0; x < 16; x++) {
      if (pattern[y][x].r == 0 && pattern[y][x].g == 0 && pattern[y][x].b == 0) {
        Serial.print("0");
      } else {
        Serial.print("1");
      }
      if (x < 15) {
        Serial.print(", ");
      }
    }
    Serial.print("}");
    if (y < 15) {
      Serial.println(",");
    } else {
      Serial.println();
    }
  }
  
  Serial.println("};");
  Serial.println();
}