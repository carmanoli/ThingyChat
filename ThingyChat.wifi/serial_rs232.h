// serial_rs232.h
#ifndef SERIAL_RS232_H
#define SERIAL_RS232_H

// worked but conflict iwth camera
#define RX_PIN D9
#define TX_PIN D10

//#define RX_PIN D3
//#define TX_PIN D4


// #define RX_PIN D6
// #define TX_PIN D7
#define BAUD_RATE 115200

void initSerial();
void handleSerial();
void sendToArduino(const char* data, size_t len = 0);

#endif

//https://github.com/espressif/arduino-esp32/blob/master/variants/XIAO_ESP32S3/pins_arduino.h
//https://community.platformio.org/t/seeed-xiao-esp32s3-pin-mapping/47599
//https://wiki.seeedstudio.com/xiao_esp32s3_pin_multiplexing/#other-hardware-serial 
//https://wiki.seeedstudio.com/xiao_esp32s3_pin_multiplexing/#other-hardware-serial