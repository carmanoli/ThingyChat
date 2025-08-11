#ifndef CONFIG_H
#define CONFIG_H

// Configurações do painel LED
#define LED_PIN     3
#define NUM_LEDS    256
#define BRIGHTNESS  50
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB


// Em Config.h
#define ACTIVE_PROJECT 0 // 0=Geiger, 1=AirQuality, etc.


const int GEIGER_INTERRUPT_PIN = 2; // Pino D3 para o sinal do Geiger
const int GEIGER_STROBE_PIN = 8;    // Pino D8 para a luz 


#include <FastLED.h>


// Tamanho do painel
constexpr int PANEL_WIDTH = 48;    // 6 matrizes de 8 colunas = 48
constexpr int PANEL_HEIGHT = 16;   // 2 linhas de 8 linhas = 16
constexpr int MATRICES_PER_ROW = 3;
constexpr int NUM_MATRICES = 6;

// Outras constantes úteis
extern const int radioactive_block[4];

CRGB getColorFromString(String colorName);

#endif