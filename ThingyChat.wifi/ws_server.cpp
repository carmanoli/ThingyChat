// ws_server.cpp
#include <Arduino.h>    
#include "ws_server.h"
#include "serial_rs232.h"

WebSocketsServer webSocket(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("[WS] Cliente #%u conectado\n", num);
      break;
    case WStype_DISCONNECTED:
      Serial.printf("[WS] Cliente #%u desconectado\n", num);
      break;
    case WStype_TEXT:
      Serial.printf("[WS] Recebido: %.*s\n", (int)length, payload);
      sendToArduino((char*)payload, length);
      break;
  }
}

void initWebSocket() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("[WS] WebSocket iniciado na porta 81");
}