#ifndef CONFIG_H
#define CONFIG_H

// Configurações do painel LED
#define LED_PIN     2 // on geugeir is 3 becau 2 is used as intrrupt
#define NUM_LEDS     256
#define BRIGHTNESS  50
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB


#define emoji16_display 3
#define PROJECT_ID_NONE               0
#define PROJECT_ID_GEIGER_COUNTER     2
#define PROJECT_ID_EMOJI16_DISPLAY    3
#define PROJECT_ID_MEMENTO_MORI       4
#define PROJECT_ID_WEATHER_STATION    5
#define PROJECT_ID_ANIMATION_PLAYER   6

#define FEATURE_NONE                 0
#define FEATURE_ANIMATION_PLAYER     (1 << 0)  // = 1
#define FEATURE_GEIGER_COUNTER       (1 << 1)  // = 2
#define FEATURE_MEMENTO_MORI         (1 << 2)  // = 4
#define FEATURE_STATIC_EMOJI_DISPLAY (1 << 3)  // = 8
#define FEATURE_EMOJI16_DISPLAY      (1 << 4)  // = 16


// --- CONFIGURAÇÃO DAS FUNCIONALIDADES ATIVAS ---
// Combine as funcionalidades que quer ativar usando o operador '|' (bitwise OR)
// #define ACTIVE_FEATURES (FEATURE_ANIMATION_PLAYER | FEATURE_GEIGER_COUNTER)
#define ACTIVE_FEATURES (FEATURE_ANIMATION_PLAYER)

// Em Config.h
//#define ACTIVE_PROJECT 0 // 0=Geiger, 1=AirQuality, etc.
//#define ACTIVE_PROJECT 1 // test leds
// #define ACTIVE_PROJECT 2 // memento_mori
//#define ACTIVE_PROJECT emoji16_display
// #define ACTIVE_PROJECT_ID PROJECT_ID_EMOJI16_DISPLAY

// #define ACTIVE_PROJECT emoji16_display
#define ACTIVE_PROJECT_ID PROJECT_ID_EMOJI16_DISPLAY


const int GEIGER_INTERRUPT_PIN = 2; // Pino D3 para o sinal do Geiger
const int GEIGER_STROBE_PIN = 8;    // Pino D8 para a luz 

#if ACTIVE_PROJECT_ID == PROJECT_ID_EMOJI16_DISPLAY
  #define ACTIVE_PROJECT_NAME "emoji16_display"
#elif ACTIVE_PROJECT_ID == PROJECT_ID_MEMENTO_MORI
  #define ACTIVE_PROJECT_NAME "memento_mori"
#elif ACTIVE_PROJECT_ID == PROJECT_ID_WEATHER_STATION
  #define ACTIVE_PROJECT_NAME "weather_station"
#elif ACTIVE_PROJECT_ID == PROJECT_ID_WEATHER_STATION
  #define ACTIVE_PROJECT_NAME "weather_station"  
#elif ACTIVE_PROJECT_ID == PROJECT_ID_ANIMATION_PLAYER
  #define ACTIVE_PROJECT_NAME "animation_player"  
#else
  #define ACTIVE_PROJECT_NAME "unknown_device"
#endif




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