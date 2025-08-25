// Ficheiro: AnimationPlayer.h (VERSÃO CORRIGIDA)
#ifndef ANIMATION_PLAYER_H
#define ANIMATION_PLAYER_H

#include <FastLED.h>
#include "VolatileAnimation.h" 

namespace AnimationPlayer {

  // A função begin agora recebe o ponteiro para os LEDs
  void begin(CRGB* leds);

  // A função play também recebe o ponteiro
  void play(CRGB* leds);

} // namespace AnimationPlayer

#endif