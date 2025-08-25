// Animation: HeartBeat
#include <FastLED.h>

#include "VolatileAnimation.h"

const int FRAME_COUNT = 3;
const int FRAME_WIDTH = 16;
const int FRAME_HEIGHT = 16;
const char* animation_direction = "pingpong"; 
const int frame_durations[FRAME_COUNT] = {400, 200, 400};

const uint8_t frame_brightness[FRAME_COUNT] = {189, 127, 69};

const CRGB animation_data[FRAME_COUNT][FRAME_HEIGHT][FRAME_WIDTH] = {
  // Frame 1
  {
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) }
  },

  // Frame 2
  {
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) }
  },

  // Frame 3
  {
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(255, 0, 0), CRGB(255, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) },
    { CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0) }
  }
};
