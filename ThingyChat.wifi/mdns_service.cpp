// File: mdns_service.cpp

#include <Arduino.h>
#include <ESPmDNS.h>
#include "mdns_service.h"

void initMDNS(const char* deviceName) {
  Serial.println("[mDNS] Initializing mDNS service...");

  if (!MDNS.begin(deviceName)) {
    Serial.println("[mDNS] Error setting up mDNS responder!");
    return;
  }

  MDNS.addService("http", "tcp", 80); // Announce the HTTP server on port 80
  
  // Announce the WebSocket service on port 81
  MDNS.addService("ws", "tcp", 81);
  
  // You could also announce an HTTP service if you have one
  // MDNS.addService("http", "tcp", 80);

  Serial.printf("[mDNS] Service started. You can now connect to http://%s.local\n", deviceName);
}