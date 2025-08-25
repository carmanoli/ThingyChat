// Ficheiro: AnimationPlayer.cpp (Substituir o conteúdo completo)

#include "AnimationPlayer.h"
#include "Config.h" 
#include "VolatileAnimation.h" // Inclui as declarações das variáveis da animação

// Variáveis de estado para controlar a reprodução
static int currentFrame = 0;
static unsigned long lastFrameTime = 0;
static int playbackDirection = 1; // 1 para a frente, -1 para trás

// A função drawFrame não muda
void drawFrame(CRGB* leds, int frameIndex) {
  // ... (o seu código de drawFrame completo aqui) ...
  const int emojiBlock[4] = {0, 1, 2, 3}; 

  for (int quadrant = 0; quadrant < 4; ++quadrant) {
    int matrixIndex = emojiBlock[quadrant];
    int startLed = matrixIndex * 64;
    int patternOffsetY = (quadrant >= 2) ? 8 : 0;
    int patternOffsetX = (quadrant % 2 != 0) ? 8 : 0;

    for (int localY = 0; localY < 8; ++localY) {
      for (int localX = 0; localX < 8; ++localX) {
        int patternY = localY + patternOffsetY;
        int patternX = localX + patternOffsetX;
        int ledIndex = startLed + (localY * 8 + localX);
        
        if (ledIndex < NUM_LEDS) {
          leds[ledIndex] = animation_data[frameIndex][patternY][patternX];
        }
      }
    }
  }
  FastLED.setBrightness(frame_brightness[frameIndex]);
  FastLED.show();
}

void AnimationPlayer::begin(CRGB* leds) {
  currentFrame = 0;
  lastFrameTime = 0; // Será atualizado no primeiro ciclo de play()
  playbackDirection = 1; // Começa sempre a andar para a frente
}

void AnimationPlayer::play(CRGB* leds) {
  // Se não houver frames, não faz nada
  if (FRAME_COUNT == 0) return;

  unsigned long now = millis();
  if (now - lastFrameTime >= frame_durations[currentFrame]) {
    lastFrameTime = now;
    
    // 1. Mostra o frame ATUAL
    drawFrame(leds, currentFrame);

    // 2. Calcula qual será o PRÓXIMO frame
    int nextFrame = currentFrame + playbackDirection;

    // 3. Verifica os limites e ajusta a direção se necessário
    if (playbackDirection == 1 && nextFrame >= FRAME_COUNT) { // Chegou ao fim a andar para a frente
      if (strcmp(animation_direction, "pingpong") == 0 && FRAME_COUNT > 1) {
        playbackDirection = -1;
        nextFrame = FRAME_COUNT - 2; // Começa a andar para trás a partir do penúltimo
      } else {
        nextFrame = 0; // Loop normal
      }
    } else if (playbackDirection == -1 && nextFrame < 0) { // Chegou ao início a andar para trás
      if (strcmp(animation_direction, "pingpong") == 0 && FRAME_COUNT > 1) {
        playbackDirection = 1;
        nextFrame = 1; // Começa a andar para a frente a partir do segundo
      } else {
        nextFrame = 0; // Loop normal (não devia acontecer, mas é seguro)
      }
    }
    
    // 4. Atualiza o estado para a próxima iteração
    currentFrame = nextFrame;
  }
}