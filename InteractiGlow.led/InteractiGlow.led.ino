#include <FastLED.h>
#include "Config.h"
#include "DisplayText.h"
#include "Emojis8.h"



CRGB leds[NUM_LEDS];  // Agora está declarado aqui

// --- Criação do objeto Geiger ---
#if ACTIVE_PROJECT == 0
  #include "Geiger.h"
  #include "Radioactive.h"
  Geiger myGeiger(GEIGER_INTERRUPT_PIN, GEIGER_STROBE_PIN);
#endif




void setup() {
  // pinMode(D3, OUTPUT);

  Serial.begin(9600);
  delay(1000);
  Serial.println("Iniciando WS2812 com FastLED...");

  Serial1.begin(115200, SERIAL_8E2);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.println("LEDs prontos!");


  Serial.println("LEDs prontos!");
  Serial.println("Comandos disponíveis:");
  Serial.println("d3h - Coloca D3 em HIGH");
  Serial.println("d3l - Coloca D3 em LOW");

  #if ACTIVE_PROJECT == 0
    myGeiger.begin();
  #endif
  Serial.println("Booting...");
}

bool booting = true;
int display = 0;
// 0 - Radioactive
// 2 - white test

int mode = 0;
// 0 - Radioactive


void loop() {
  if (booting) {
    booting = false;
    clearAllMatrices();

    if (display == 2) {
      for (int i = 0; i < NUM_LEDS; i++) {

        leds[i] = getColorFromString("WHITE");
        FastLED.show();

        Serial.print("Testing Led: ");
        Serial.println(i);
      }
    }

    if (display == 1) {
      displayString("CARLOS", 0);
      displayString("OLIVEIRA", 1);
    }

    if (mode == 0) {
      drawRadioactiveSymbol16x16(leds, NUM_LEDS, radioactive_block);
      FastLED.show();
      delay(10);
    }
  }

  #if ACTIVE_PROJECT == 0
    myGeiger.update();
    // E aqui a lógica para lidar com os dados do Geiger
    myGeiger.handle(Serial1);
  #elif ACTIVE_PROJECT == 1
    // myAirQuality.update();
    // handleAirQualityMode();
  #endif



  if (mode == 0) {

    unsigned long currentCPM = myGeiger.getCPM();
    if (currentCPM > 100) {
      Serial1.println("GEIGER_LEVEL:2");
    } else if (currentCPM > 50) {
      Serial1.println("GEIGER_LEVEL:1");
    }
  }


  if (Serial.available()) {
    String comandoUSB = Serial.readStringUntil('\n');
    comandoUSB.trim();  // Remove espaços e quebras de linha

/*
    // Verifica comandos para o pino D3
    if (comandoUSB == "d3h") {
      digitalWrite(D3, HIGH);
      Serial.println("D3 set to HIGH");
    } else if (comandoUSB == "d3l") {
      digitalWrite(D3, LOW);
      Serial.println("D3 set to LOW");
    } else {
      // Se não for um comando local, envia para o Serial1
      Serial1.println(comandoUSB);
    }
    */
  }

  if (Serial1.available()) {
    String comandoXiao = Serial1.readStringUntil('\n');
    Serial.print("comandoXiao:");
    Serial.println(comandoXiao);

    comandoXiao.replace("\r", "");
    comandoXiao.replace("\n", "");

    if (comandoXiao == "HEART") {
      Serial.println("Display Heart");
      drawEmojiOnMatrix(leds, NUM_LEDS, 0, heartPattern, CRGB::Red);
      drawEmojiOnMatrix(leds, NUM_LEDS, 1, heartPattern, CRGB::Green);
    }
  }
}
