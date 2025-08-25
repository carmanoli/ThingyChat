// File: device_comms.cpp
#include "device_comms.h"

String getDeviceNameFromArduino(unsigned long timeoutMillis) {
  Serial.println("[Handshake] Asking Arduino for its name...");
  Serial.println("GET_NAME"); // Send the command to the Arduino

  unsigned long startTime = millis();
  while (millis() - startTime < timeoutMillis) {
    if (Serial.available() > 0) {
      String response = Serial.readStringUntil('\n');
      response.trim();
      
      if (response.startsWith("NAME:")) {
        String deviceName = response.substring(5); // Get the text after "NAME:"
        Serial.printf("[Handshake] Received name: %s\n", deviceName.c_str());
        return deviceName;
      }
    }
    delay(10); // Small delay to prevent busy-waiting
  }

  Serial.println("[Handshake] Timeout! No response from Arduino.");
  return ""; // Return empty string on failure
}