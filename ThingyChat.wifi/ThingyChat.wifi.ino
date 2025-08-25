#include "config.h"
#include "wifi_config.h"
#include "wifi_ap.h"
#include "serial_rs232.h"
#include "mdns_service.h"
#include "device_comms.h"
#include "esp_system.h"
#include "http_ws_server.h"
#include "Camera.h"
#include <esp_camera.h>

bool apMode = false;
String deviceName = "generic-thingy";
bool handshakeCompleted = false;
bool servicesStarted = false;

bool tryGetDeviceName() {
  if (Serial1.available() > 0) {
    String response = Serial1.readStringUntil('\n');
    response.trim();
    Serial.printf("[Xiao] Recebido do Arduino: '%s'\n", response.c_str());
    if (response.length() > 0) {
      Serial.printf("[Xiao] Resposta válida após trim: '%s'\n", response.c_str());
      if (response.startsWith("NAME:")) {
        deviceName = response.substring(5);
        Serial.printf("[Xiao] Nome do dispositivo recebido: %s\n", deviceName.c_str());
        while (Serial1.available()) Serial1.read();
        return true;
      } else {
        Serial.printf("[Xiao] Resposta não começa com 'NAME:': '%s'\n", response.c_str());
      }
    } else {
      Serial.println("[Xiao] Resposta vazia após trim.");
    }
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n[Xiao] Iniciando...");

  // Inicializar Serial1 em novos pinos
  //Serial1.begin(115200, SERIAL_8N1, 3, 4); // RX = GPIO 3 (D2), TX = GPIO 4 (D3)

  // Inicializar câmera
  bool camera_ok = Camera::begin();
  if (!camera_ok) {
    Serial.println("[Xiao] Falha na inicialização da câmera! Verifique os logs acima.");
  }

  // Teste da câmera
  Serial.println("[Xiao] Testando captura de imagem...");
  camera_fb_t *fb = esp_camera_fb_get();
  if (fb) {
    Serial.printf("[Xiao] Imagem capturada com sucesso: %d bytes, %dx%d\n", fb->len, fb->width, fb->height);
    esp_camera_fb_return(fb);
  } else {
    Serial.println("[Xiao] Falha ao capturar imagem no setup!");
    sensor_t *s = esp_camera_sensor_get();
    if (s) {
      Serial.printf("[Xiao] Sensor ativo, ID: 0x%x\n", s->id.PID);
    } else {
      Serial.println("[Xiao] Sensor da câmera não detectado!");
    }
  }

  // Tentar obter o nome do dispositivo do Arduino
  Serial.println("[Xiao] Enviando 'GET_NAME' para o Arduino...");
  Serial1.println("GET_NAME");
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    if (tryGetDeviceName()) {
      handshakeCompleted = true;
      break;
    }
    delay(10);
  }

  if (!handshakeCompleted) {
    Serial.printf("[Xiao] Handshake falhou. Usando nome por defeito: %s\n", deviceName.c_str());
  }

  // Configuração da rede
  if (connectToWiFi()) {
    apMode = false;
    Serial.println("[Xiao] Conectado ao WiFi com sucesso.");
    if (handshakeCompleted) {
      initHttpAndWsServer(deviceName);
      initMDNS(deviceName.c_str());
      servicesStarted = true;
      Serial.println("[Xiao] Serviços WebSocket e mDNS iniciados.");
    } else {
      Serial.println("[Xiao] Aguardando nome do dispositivo para iniciar serviços de rede...");
    }
  } else {
    apMode = true;
    Serial.println("[Xiao] Falha ao conectar ao WiFi. Iniciando Modo AP.");
    startAPMode();
  }

  initSerial();
}

void loop() {
  if (!handshakeCompleted) {
    Serial1.println("GET_NAME");
    if (tryGetDeviceName()) {
      handshakeCompleted = true;
      if (!apMode && !servicesStarted) {
        initHttpAndWsServer(deviceName);
        initMDNS(deviceName.c_str());
        servicesStarted = true;
        Serial.println("[Xiao] Serviços WebSocket e mDNS iniciados com nome recebido.");
      }
    }
    delay(1000);
  }

  if (handshakeCompleted) {
    handleSerial();
  }

  if (apMode) {
    handleAPClient();
  } else if (servicesStarted) {
    serverLoop();
  }
}