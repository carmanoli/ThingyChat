// wifi_config.h
#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Preferences.h>

void saveWiFiCredentials(const String& ssid, const String& pass);
bool loadWiFiCredentials(String& ssid, String& pass);
bool connectToWiFi();
void resetWiFiCredentials();

#endif