// #include <FastLED.h>
#include "Config.h"  // Onde ACTIVE_PROJECT_ID e ACTIVE_PROJECT_NAME estão definidos
#include "DisplayText.h"
#include "Emojis8.h"
#include "Emojis16.h"
#include "core_cm4.h"


#if (ACTIVE_FEATURES & FEATURE_ANIMATION_PLAYER)
  #include "AnimationPlayer.h"
  // #include "VolatileAnimation.cpp"
#endif

#if (ACTIVE_FEATURES & FEATURE_GEIGER_COUNTER)
#include "Geiger.h"
#include "Radioactive.h"
Geiger myGeiger(GEIGER_INTERRUPT_PIN, GEIGER_STROBE_PIN);
#endif




CRGB leds[NUM_LEDS];

// --- VARIÁVEL GLOBAL PARA RASTREAR O HANDSHAKE ---
bool handshakeCompleted = false;  // Indica se o handshake foi concluído


void softwareReset() {
  NVIC_SystemReset();
}

// --- FUNÇÃO DE HANDSHAKE ---
void handleHandshake() {
  if (Serial1.available() > 0) {
    String command = Serial1.readStringUntil('\n');
    command.trim();
    if (command.length() > 0) {  // Verifica se o comando não está vazio
      Serial.print("Arduino: Comando recebido: ");
      Serial.println(command);
      if (command == "GET_NAME") {
        String response = "NAME:" + String(ACTIVE_PROJECT_NAME);
        Serial1.println(response);  // Responde imediatamente
        Serial.print("Arduino: Handshake OK! Nome enviado: ");
        Serial.println(response);
        handshakeCompleted = true;                   // Marca o handshake como concluído
        while (Serial1.available()) Serial1.read();  // Limpa o buffer
      }
    }
  }
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1);  // Velocidade de comunicação com o Xiao
  delay(1000);                        // Dar tempo para as portas seriais estabilizarem

  Serial.println("Arduino: Iniciando...");

  // Tenta o handshake no arranque (com timeout)
  Serial.println("Arduino: Aguardando 'GET_NAME' do Xiao...");
  unsigned long startTime = millis();
  while (millis() - startTime < 6000) {
    handleHandshake();
    if (handshakeCompleted) {
      break;
    }
    delay(10);  // Pequena pausa
  }
  if (!handshakeCompleted) {
    Serial.println("Arduino: Handshake falhou (timeout). Continuando a verificar no loop...");
  }

  // Continua com o resto do setup
  Serial.println("Arduino: Iniciando FastLED...");
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Arduino: LEDs prontos!");

  #if (ACTIVE_FEATURES & FEATURE_ANIMATION_PLAYER)
    Serial.println("Arduino: A iniciar o Animation Player...");
    AnimationPlayer::begin(leds); // <-- CORRIGIDO: Passa o 'leds'
  #endif
  #if (ACTIVE_FEATURES & FEATURE_GEIGER_COUNTER)
    Serial.println("Arduino: A iniciar o Geiger Counter...");
    myGeiger.begin();
  #endif
}

// --- LOOP PRINCIPAL ---
bool hasBooted = false;

void loop() {
  // Continua respondendo a GET_NAME mesmo após o handshake
  handleHandshake();

  // Lógica que corre apenas uma vez após o setup
  if (!hasBooted) {
    hasBooted = true;
    clearAllMatrices();
    Serial.println("Arduino: Lógica de arranque a correr...");

    #if (ACTIVE_FEATURES & FEATURE_EMOJI16_DISPLAY)
      const int emojiBlock[4] = { 0, 1, 2, 3 };
      drawEmoji16x16_Animated(leds, NUM_LEDS, emojiBlock, smileyFace16, 10);
    #endif

    #if (ACTIVE_FEATURES & FEATURE_MEMENTO_MORI)
        MementMori::drawEgg(leds, 0, NUM_LEDS);
        MementMori::drawHeart(leds, 1, NUM_LEDS);
        MementMori::drawSkull(leds, 2, NUM_LEDS);
        FastLED.show();
    #endif



#if (ACTIVE_FEATURES & FEATURE_GEIGER_COUNTER)
    drawRadioactiveSymbol16x16(leds, NUM_LEDS, radioactive_block);
    FastLED.show();

#endif



    Serial.println("Arduino: Lógica de arranque concluída.");
  }

  // Lógica contínua do loop

  #if (ACTIVE_FEATURES & FEATURE_ANIMATION_PLAYER)
      AnimationPlayer::play(leds); // <-- CORRIGIDO: Passa o 'leds'
  #endif
  #if (ACTIVE_FEATURES & FEATURE_GEIGER_COUNTER)
    myGeiger.update();
    myGeiger.handle(Serial1);
  #endif


  // Ouve por comandos do Xiao (exceto GET_NAME, que é tratado por handleHandshake)
  if (Serial1.available()) {
    String comandoXiao = Serial1.readStringUntil('\n');
    comandoXiao.trim();
    if (comandoXiao.length() > 0 && comandoXiao != "GET_NAME") {  // Ignora GET_NAME
      Serial.print("Arduino: Comando do Xiao -> ");
      Serial.println(comandoXiao);
      if (comandoXiao == "HEART") {
        Serial.println("Arduino: A desenhar Coração...");
        // drawEmojiOnMatrix(leds, NUM_LEDS, 0, heartPattern, CRGB::Red);
      }
    }
  }

  // Ouve por comandos do Monitor Serial (para debug) e reencaminha para o Xiao
  if (Serial.available()) {
    String comandoUSB = Serial.readStringUntil('\n');
    comandoUSB.trim();
    if (comandoUSB == "reset") {
      Serial.println("Resetting...");
      softwareReset();  // Enable watchdog for 15ms
      while (1)
        ;  // Wait for reset
    } else {
      Serial.print("Arduino->Xiao:");
      Serial.println(comandoUSB);
      Serial1.println(comandoUSB);
    }
  }
}