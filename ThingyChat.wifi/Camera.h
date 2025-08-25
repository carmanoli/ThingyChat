// Ficheiro: Camera.h (Remover a palavra 'class')
#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>

// A classe completa será conhecida através de outros includes, por isso não precisamos da forward declaration
class AsyncWebSocketClient; // Mantém a forward declaration para clareza e compilação independente

namespace Camera {
  bool begin();
  void captureAndSendPixelated(int gridSize, AsyncWebSocketClient *client);
}

#endif