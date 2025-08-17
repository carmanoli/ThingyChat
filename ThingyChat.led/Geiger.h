#ifndef GEIGER_H
#define GEIGER_H

#include <Arduino.h>

class Geiger {
public:
  // Construtor: Inicializa o módulo Geiger
  Geiger(int interruptPin, int strobePin);

  // Inicializa o contador (deve ser chamado no setup() principal)
  void begin();

  // Função de atualização (deve ser chamada no loop() principal)
  void update();

  // Retorna o último valor de CPM calculado
  unsigned long getCPM();

  void handle(HardwareSerial &commSerial); 

private:
  // Pino para a interrupção do tubo Geiger (D2 na maioria dos exemplos)
  int _interruptPin;
  // Pino para controlar o LED/luz estroboscópica (opcional)
  int _strobePin;

  // Variável volátil para a contagem, pois é modificada por uma interrupção
  static volatile unsigned long _counts;

  // Variáveis para o cálculo do CPM
  unsigned long _cpm;
  unsigned long _previousMillis;

  // Constantes de tempo
  static const unsigned long LOG_PERIOD = 5000; // Período de cálculo em ms (5 segundos)
  static const unsigned long ONE_MINUTE_IN_MS = 60000;

  // A função que será chamada pela interrupção (ISR)
  // Tem de ser estática para ser usada com attachInterrupt
  static void tube_impulse(); 

    // Nova variável para evitar spam
  unsigned long _lastCPM_sent; 
};

#endif // GEIGER_H