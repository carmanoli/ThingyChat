#include "http_ws_server.h"
#include <ESPAsyncWebServer.h>
#include "serial_rs232.h"
#include "Camera.h"


static AsyncWebServer server(80);
static AsyncWebSocket ws("/ws");
static String wsDeviceName = "unknown";

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("[WS] Client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      client->text("DEVICE:" + wsDeviceName);
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("[WS] Client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA: {
      AwsFrameInfo *info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        String msg = "";
        for (size_t i = 0; i < len; i++) msg += (char)data[i];
        Serial.printf("[WS] Received from client #%u: %s\n", client->id(), msg.c_str());
        if (msg.startsWith("TAKE_PIXEL_PHOTO:")) {
          Serial.println("[WS] Processing TAKE_PIXEL_PHOTO command...");
          int gridSize = msg.substring(17).toInt();
          Serial.printf("[WS] Grid Size: %d\n", gridSize);
          if (gridSize > 0) {
            Serial.println("[WS] Calling Camera::captureAndSendPixelated...");
            Camera::captureAndSendPixelated(gridSize, client);
            Serial.println("[WS] Returned from Camera::captureAndSendPixelated.");
          } else {
            Serial.println("[WS] Invalid grid size!");
            client->text("ERROR:Invalid grid size.");
          }
        } else {
          Serial.printf("[WS] Sending to Arduino: %s\n", msg.c_str());
          sendToArduino(msg.c_str(), msg.length());
        }
      }
      break;
    }
    case WS_EVT_ERROR:
      Serial.printf("[WS] Error on client #%u\n", client->id());
      break;
  }
}

void initHttpAndWsServer(String deviceName) {
  wsDeviceName = deviceName;
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "ThingyChat Device is online. Connect via WebSocket on /ws");
  });
  server.begin();
  Serial.println("[HTTP/WS] Server started.");
}

void serverLoop() {
  ws.cleanupClients();
}