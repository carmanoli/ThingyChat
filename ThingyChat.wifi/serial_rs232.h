// serial_rs232.h
#ifndef SERIAL_RS232_H
#define SERIAL_RS232_H

#define RX_PIN 16
#define TX_PIN 17
#define BAUD_RATE 115200

void initSerial();
void handleSerial();
void sendToArduino(const char* data, size_t len = 0);

#endif