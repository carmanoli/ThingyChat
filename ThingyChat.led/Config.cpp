// Config.cpp
#include "Config.h"

const int radioactive_block[4] = {0, 1, 2, 3};

// Color conversion function
CRGB getColorFromString(String colorName) {
  colorName.toUpperCase();
  if (colorName == "RED") return CRGB::Red;
  if (colorName == "BLUE") return CRGB::Blue;
  if (colorName == "GREEN") return CRGB::Green;
  if (colorName == "WHITE") return CRGB::White;
  if (colorName == "PURPLE") return CRGB::Purple;
  if (colorName == "YELLOW") return CRGB::Yellow;
  if (colorName == "CYAN") return CRGB::Cyan;
  if (colorName == "ORANGE") return CRGB::Orange;
  return CRGB::White; // Default color
}

