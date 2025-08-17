#include "DisplayText.h"
#include <Arduino.h>
#include "Config.h"  // Aqui você acessa NUM_LEDS

extern CRGB leds[NUM_LEDS]; // Declarado no .ino ou globalmente

void displayString(const String& text, int line, CRGB color) {
  String textUpper = text;
  textUpper.toUpperCase();

  int textLength = textUpper.length();
  if (textLength == 0 || (4 * textLength - 1) > PANEL_WIDTH) {
    Serial.println("Erro: String muito longa ou vazia para caber no painel");
    return;
  }

  for (int i = 0; i < textLength; i++) {
    if (textUpper[i] < 'A' || textUpper[i] > 'Z') {
      Serial.println("Erro: String contém caracteres inválidos (apenas A-Z)");
      return;
    }
  }

  int startRow = (line == 1) ? 7 : 0;
  if (startRow + 5 > PANEL_HEIGHT) {
    Serial.println("Erro: Linha inválida (0 ou 1)");
    return;
  }

  for (int i = 0; i < textLength; i++) {
    int letterIndex = textUpper[i] - 'A';
    int startCol = i * 4;

    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 3; x++) {
        int panelCol = startCol + x;
        int panelRow = startRow + y;

        int matrixIndex = (panelRow / 8) * MATRICES_PER_ROW + (panelCol / 8);
        int localRow = panelRow % 8;
        int localCol = panelCol % 8;
        int ledIndex = matrixIndex * 64 + localRow * 8 + localCol;

        if (ledIndex >= 0 && ledIndex < NUM_LEDS) {
          leds[ledIndex] = (alphabet[letterIndex][y][x] == 1) ? color : CRGB::Black;
        }
      }
    }
  }
  FastLED.show();
}

void displayCarlos() {
  displayString("CARLOS", 0, CRGB::Blue);
}

void clearAllMatrices() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}