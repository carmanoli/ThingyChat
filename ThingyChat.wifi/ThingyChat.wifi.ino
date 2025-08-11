// ThingyChat.wifi.ino

#include "wifi_config.h"
#include "wifi_ap.h"
#include "ws_server.h"
#include "serial_rs232.h"

bool apMode = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("\n[ThingyChat.wifi] Iniciando...");

  // --- FORÇAR MODO AP PARA TESTE ---
  Serial.println("[DEBUG] Forçando modo AP para teste!");
  startAPMode();
  apMode = true;
  // ----------------------------------

  initSerial();
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
  if (millis() - prev >= 5000) {
    Serial.println("keepAlive");
    prev = millis();
  }
}