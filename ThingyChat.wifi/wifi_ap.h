// wifi_ap.h
#ifndef WIFI_AP_H
#define WIFI_AP_H

#include <WebServer.h>
#include <DNSServer.h>

// Declara como extern — a definição está em wifi_ap.cpp
extern WebServer server;
extern DNSServer dnsServer;

void startAPMode();
void handleAPClient();



#endif