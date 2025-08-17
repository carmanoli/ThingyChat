#include "Geiger.h"

// Inicialização da variável estática volátil.
// Essencial para que a ISR funcione corretamente.
volatile unsigned long Geiger::_counts = 0;
static int _isr_strobe_pin;

// Construtor: apenas guarda os pinos que vamos usar
Geiger::Geiger(int interruptPin, int strobePin) {
  _interruptPin = interruptPin;
  _strobePin = strobePin;
  _isr_strobe_pin = _strobePin;
  _cpm = 0;
  _previousMillis = 0;
}

// ISR (Interrupt Service Routine): A função que é executada a cada pulso do tubo
// É super rápida e só faz duas coisas: incrementa a contagem e pisca a luz.
void Geiger::tube_impulse() {
  _counts++;
  // Para piscar o LED estroboscópico, teríamos que passar o pino aqui
  // ou usar uma abordagem diferente. Por agora, vamos manter simples.
    digitalWrite(_isr_strobe_pin, HIGH);
  delayMicroseconds(100); // Um piscar muito curto
  digitalWrite(_isr_strobe_pin, LOW);
}

// begin(): Configura os pinos e a interrupção
void Geiger::begin() {
  // Apenas INPUT é suficiente, já que o sinal é forte.
  pinMode(_interruptPin, INPUT); 
  
  pinMode(_strobePin, OUTPUT);
  digitalWrite(_strobePin, LOW);

  // A mudança crucial: usar RISING
  attachInterrupt(digitalPinToInterrupt(_interruptPin), tube_impulse, RISING); // <<< MUDADO
  
  Serial.println("Módulo Geiger iniciado no modo RISING. A contar pulsos...");
}

// update(): A função principal a ser chamada no loop. Calcula o CPM periodicamente.
void Geiger::update() {
  unsigned long currentMillis = millis();

  // A cada LOG_PERIOD (15 segundos), calcula o CPM
  if (currentMillis - _previousMillis >= LOG_PERIOD) {
    _previousMillis = currentMillis;

    // Desativa temporariamente as interrupções para ler _counts de forma segura
    // Isto evita que a ISR mude o valor enquanto o estamos a ler.
    noInterrupts();
    unsigned long currentCounts = _counts;
    _counts = 0; // Reinicia a contagem para o próximo período
    interrupts(); // Reativa as interrupções

    // Calcula o CPM
    // Multiplicador = (60 segundos / período de log em segundos)
    unsigned int multiplier = ONE_MINUTE_IN_MS / LOG_PERIOD;
    _cpm = currentCounts * multiplier;

    // Mostra as leituras no Serial Monitor (como pedido)
    Serial.print("Leitura do período: ");
    Serial.print(currentCounts);
    Serial.print(" pulsos. CPM calculado: ");
    Serial.println(_cpm);
  }
}

// getCPM(): Função simples para obter o último valor calculado
unsigned long Geiger::getCPM() {
  return _cpm;
}

// Implementação da nova função 'handle'
void Geiger::handle(HardwareSerial &commSerial) {
  // A lógica que estava no .ino agora está aqui dentro.
  unsigned long currentCPM = getCPM();

  if (currentCPM != _lastCPM_sent) {
    _lastCPM_sent = currentCPM; // Atualiza o último valor enviado

    String msg = "GEIGER_LEVEL:";
    if (currentCPM > 100) {
      commSerial.println(msg + "2");
    } else if (currentCPM > 50) {
      commSerial.println(msg + "1");
    } else {
      commSerial.println(msg + "0");
    }
  }
}

