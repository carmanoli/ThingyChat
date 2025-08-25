// Ficheiro: VolatileAnimation.h (Adicionar a nova declaração)

#ifndef VOLATILE_ANIMATION_H
#define VOLATILE_ANIMATION_H

#include <FastLED.h>

extern const int FRAME_COUNT;
extern const int FRAME_WIDTH;
extern const int FRAME_HEIGHT;
extern const char* animation_direction; // <-- ADICIONAR ESTA LINHA
extern const int frame_durations[];
extern const uint8_t frame_brightness[];
extern const CRGB animation_data[][16][16];

#endif