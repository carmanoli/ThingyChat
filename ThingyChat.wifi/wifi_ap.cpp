// wifi_ap.cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "wifi_ap.h"
#include "wifi_config.h"

// ✅ DEFINIÇÃO GLOBAL (único lugar!)
WebServer server(80);        // Porta 80
DNSServer dnsServer;         // Servidor DNS para captive portal

void startAPMode() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  bool success = WiFi.softAP("wifi_ap", "password");
  if (!success) {
    Serial.println("[AP] Falha ao criar AP");
    return;
  }

  IPAddress apIP(192, 168, 4, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  dnsServer.start(53, "*", apIP);  // Captive portal
  server.begin();

  Serial.print("[AP] Acesse: http://");
  Serial.println(apIP);
}

void handleAPClient() {
  dnsServer.processNextRequest();
  server.handleClient();
}