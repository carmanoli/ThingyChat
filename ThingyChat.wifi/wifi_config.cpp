// wifi_config.cpp
#include <Arduino.h>    
#include "wifi_config.h"
#include <WiFi.h>

Preferences preferences;

void saveWiFiCredentials(const String& ssid, const String& pass) {
  preferences.begin("wifi", false);
  preferences.putString("ssid", ssid);
  preferences.putString("pass", pass);
  preferences.end();
  Serial.println("[WiFi] Credenciais salvas.");
}

bool loadWiFiCredentials(String& ssid, String& pass) {
  preferences.begin("wifi", false);
  ssid = preferences.getString("ssid", "");
  pass = preferences.getString("pass", "");
  preferences.end();
  return !ssid.isEmpty();
}

bool connectToWiFi() {
  String ssid, pass;
  if (!loadWiFiCredentials(ssid, pass)) return false;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  Serial.print("[WiFi] Conectando a: ");
  Serial.println(ssid);

  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout-- > 0) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] IP: " + WiFi.localIP().toString());
    return true;
  }

  Serial.println("\n[WiFi] Falha ao conectar.");
  return false;
}

void resetWiFiCredentials() {
  preferences.begin("wifi", false);
  preferences.clear();
  preferences.end();
}