// serial_rs232.cpp
#include <Arduino.h>           // ✅ Obrigatório em todos .cpp!
#include "serial_rs232.h"

void initSerial() {
  Serial1.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Serial inicializado.");
}

void handleSerial() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    sendToArduino(cmd.c_str());
  }

  if (Serial1.available()) {
    String cmd = Serial1.readStringUntil('\n');
    Serial.print("comandoArduino: ");
    Serial.println(cmd);
  }
}

void sendToArduino(const char* data, size_t len) {
  if (len > 0) {
    Serial1.write((uint8_t*)data, len);
    Serial1.write('\n');
  } else {
    Serial1.println(data);
  }
}