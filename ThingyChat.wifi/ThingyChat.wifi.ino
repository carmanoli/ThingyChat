#include "config.h"

// ThingyChat.wifi.ino

#include "wifi_config.h"
#include "wifi_ap.h"
#include "ws_server.h"
#include "serial_rs232.h"

#ifdef FEATURE_NFC_ENABLED
  //#include "nfc.h"
#endif


bool apMode = false;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n[ThingyChat] Iniciando...");
  
  if (connectToWiFi()) {
    // SUCESSO! Conectou-se como cliente (STA mode)
    apMode = false;
    initWebSocket(); // Inicia o WebSocket SÓ se estiver em modo cliente
  } else {
    // FALHA! Não há credenciais guardadas ou a rede está fora de alcance.
    // Inicia o modo Access Point (AP) para configuração.
    apMode = true;
    startAPMode();
    // NOTA: O servidor web para a página de configuração deve ser iniciado dentro de startAPMode()
  }
  initSerial();
/*
  #ifdef FEATURE_NFC_ENABLED
    initNFC();
    Serial.println("\nAproxime uma tag NFC ou um cartão...");
  #endif
*/
}

void loop() {
  handleSerial();

  if (apMode) {
    handleAPClient();  // AP + Web Server
  } else {
    webSocket.loop();  // WebSocket
  }




  // keepAlive
  static unsigned long prev = 0;
  if (millis() - prev >= 30000) {
    Serial.println("keepAlive");
    prev = millis();
  }
/*
  #ifdef FEATURE_NFC_ENABLED
    handleNFC_Test();
  #endif
*/
}