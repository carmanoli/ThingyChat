#include <FastLED.h>
#include "Config.h"
#include "DisplayText.h"
#include "Emojis8.h"
#include "Emojis16.h"


CRGB leds[NUM_LEDS];  // Agora está declarado aqui

// --- Criação do objeto Geiger ---
#if ACTIVE_PROJECT == 0
  #include "Geiger.h"
  #include "Radioactive.h"
  Geiger myGeiger(GEIGER_INTERRUPT_PIN, GEIGER_STROBE_PIN);
#endif

#if ACTIVE_PROJECT == 2  
  #include "MementoMori.h"
#endif




void setup() {
  // pinMode(D3, OUTPUT);

  Serial.begin(9600);
  delay(1000);
  Serial.println("Iniciando WS2812 com FastLED...");

  Serial1.begin(9600, SERIAL_8N1);

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


    #if ACTIVE_PROJECT == 1 // Test leds
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = getColorFromString("WHITE");
        FastLED.show();

        Serial.print("Testing Led: ");
        Serial.println(i);
      }
    
    #endif


    if (display == 1) {
      displayString("CARLOS", 0);
      displayString("OLIVEIRA", 1);
    }


    #if ACTIVE_PROJECT == 0
      drawRadioactiveSymbol16x16(leds, NUM_LEDS, radioactive_block);
      FastLED.show();
      delay(10);
    #endif

    #if ACTIVE_PROJECT == 2
    FastLED.clear();
    
    // Draw emojis on each matrix
    MementMori::drawEgg(leds, 0, NUM_LEDS);    // First matrix (index 0)
    MementMori::drawHeart(leds, 1, NUM_LEDS);  // Second matrix (index 1)
    MementMori::drawSkull(leds, 2, NUM_LEDS);  // Third matrix (index 2)
    
    FastLED.show();
    #endif


    #if ACTIVE_PROJECT == 3
      // Defina quais matrizes físicas formarão o seu ecrã 16x16
      
      // Exemplo: Matrizes 0, 1 (linha de cima) e 3, 4 (linha de baixo correspondente)
      // A ordem é: [Topo-Esquerdo, Topo-Direito, Baixo-Esquerdo, Baixo-Direito]
      const int emojiBlock[4] = {0, 1, 2, 3}; // <<< AJUSTE ESTES ÍNDICES para a sua fiação

      // Chama a nova função genérica para desenhar o smiley
      //drawEmoji16x16(leds, NUM_LEDS, emojiBlock, smileyFace16);
      //FastLED.show(); // Atualiza o painel de LEDs

      int animationSpeed = 10; 

      // Call the NEW animated function
      drawEmoji16x16_Animated(leds, NUM_LEDS, emojiBlock, smileyFace16, animationSpeed);
    #endif


  }

  #if ACTIVE_PROJECT == 0
    myGeiger.update();
    // E aqui a lógica para lidar com os dados do Geiger
    myGeiger.handle(Serial1);
    unsigned long currentCPM = myGeiger.getCPM();
    if (currentCPM > 100) {
      Serial1.println("GEIGER_LEVEL:2");
    } else if (currentCPM > 50) {
      Serial1.println("GEIGER_LEVEL:1");
    }


  #elif ACTIVE_PROJECT == 1
    // myAirQuality.update();
    // handleAirQualityMode();
  #endif





  if (Serial.available()) {
    String comandoUSB = Serial.readStringUntil('\n');
    comandoUSB.trim();  // Remove espaços e quebras de linha
      Serial1.println(comandoUSB);

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
