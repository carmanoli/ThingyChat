#ifndef DISPLAYTEXT_H
#define DISPLAYTEXT_H

#include <FastLED.h>
#include "Alphabet.h"
#include "Config.h"  // Agora tem acesso a NUM_LEDS

void displayString(const String& text, int line, CRGB color = CRGB::White);
void displayCarlos();
void clearAllMatrices();

#endif