// radioactive.h

#ifndef RADIOACTIVE_H
#define RADIOACTIVE_H

#include <FastLED.h>

/**
 * @brief Desenha um único símbolo radioativo de 16x16 (preto sobre amarelo)
 *        distribuído por um bloco de 4 matrizes de LED 8x8 (dispostas em 2x2).
 * 
 * @param leds O ponteiro para o array de LEDs (ex: CRGB leds[NUM_LEDS]).
 * @param numLeds O número total de LEDs no array, para segurança.
 * @param matrixIndices Um array de 4 inteiros que especifica os índices das matrizes físicas
 *                      que formarão o bloco 2x2. A ordem é crucial:
 *                      - matrixIndices[0]: Matriz do Topo-Esquerdo
 *                      - matrixIndices[1]: Matriz do Topo-Direito
 *                      - matrixIndices[2]: Matriz de Baixo-Esquerdo
 *                      - matrixIndices[3]: Matriz de Baixo-Direito
 */
void drawRadioactiveSymbol16x16(CRGB* leds, int numLeds, const int matrixIndices[4]);

#endif // RADIOACTIVE_H