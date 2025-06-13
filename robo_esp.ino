#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig' to and enable it
#endif

BluetoothSerial SerialBT;


// Configuração dos pinos
const int IN1 = 25;  // Motor ESQUERDO frente
const int IN2 = 26;  // Motor ESQUERDO trás
const int IN3 = 32;  // Motor DIREITO frente
const int IN4 = 33;  // Motor DIREITO trás
const int pinIR = 27;  // Sensor infravermelho (ativa no branco)

// Comandos Bluetooth
const char COMANDO_AVANCAR = 'a';
const char COMANDO_ESQUERDA = 'e';
const char COMANDO_DIREITA = 'd';
const char COMANDO_RECUAR = 'r';
const char COMANDO_PARAR = 'p';
const char COMANDO_MEIAVOLTA = 'm';

// Variáveis de controle
char comandoAtual = COMANDO_PARAR;
bool brancoDetectado = false;
unsigned long ultimaDetecao = 0;
const unsigned long intervaloIR = 200;  // Debounce para sensor IR
const unsigned long tempoReacao = 1000; // Tempo de manobra após detecção

// Configuração específica para sensor que ativa no branco (LOW quando detecta branco)
const int LIMIAR_BRANCO = LOW;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Tucunaré"); 
  Serial.println("Robô iniciado - Controle por Bluetooth habilitado");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(pinIR, INPUT);
  
  parar(); // Garante que inicia parado
}

// ========== FUNÇÕES DE MOVIMENTO ==========
void avancarFrente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  comandoAtual = COMANDO_AVANCAR;
  Serial.println("Movimento: Frente");
}

void girarEsquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  comandoAtual = COMANDO_ESQUERDA;
  Serial.println("Movimento: Esquerda");
}

void girarDireita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  comandoAtual = COMANDO_DIREITA;
  Serial.println("Movimento: Direita");
}

void recuar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  comandoAtual = COMANDO_RECUAR;
  Serial.println("Movimento: Ré");
}

void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  comandoAtual = COMANDO_PARAR;
  Serial.println("Movimento: Parado");
}

void meiaVolta() {
  Serial.println("Executando meia-volta...");
  parar();
  delay(150);
  recuar();
  delay(500);
  girarDireita();
  delay(400);
  parar();
}

// ========== FUNÇÕES DO SENSOR IR (ATIVA NO BRANCO) ==========
bool verificarBranco() {
  int leituraIR = digitalRead(pinIR);
  
  // Verifica se detectou superfície branca (LOW)
  bool deteccao = (leituraIR == LIMIAR_BRANCO);
  
  if (deteccao && !brancoDetectado && (millis() - ultimaDetecao > intervaloIR)) {
    brancoDetectado = true;
    ultimaDetecao = millis();
    Serial.println("Superfície branca detectada pelo sensor IR!");
    return true;
  }
  
  // Reset da detecção após intervalo
  if (!deteccao && (millis() - ultimaDetecao > tempoReacao)) {
    brancoDetectado = false;
  }
  
  return false;
}

void reagirBranco() {
  if (verificarBranco() && comandoAtual == COMANDO_AVANCAR) {
    meiaVolta();
    // Pode adicionar lógica alternativa aqui, como:
    // - Girar 90 graus
    // - Procurar outra direção
    // - Seguir linha se for um seguidor de linha
  }
}

// ========== CONTROLE BLUETOOTH ==========
void processarComandosBT() {
  if (SerialBT.available()) {
    char comando = SerialBT.read();
    Serial.print("Comando recebido: ");
    Serial.println(comando);
    
    switch(comando) {
      case COMANDO_AVANCAR: avancarFrente(); break;
      case COMANDO_ESQUERDA: girarEsquerda(); break;
      case COMANDO_DIREITA: girarDireita(); break;
      case COMANDO_RECUAR: recuar(); break;
      case COMANDO_PARAR: parar(); break;
      case COMANDO_MEIAVOLTA: meiaVolta(); break;
      default: 
        Serial.println("Comando inválido");
        break;
    }
  }
}

// ========== LOOP PRINCIPAL ==========
void loop() {
  reagirBranco();       // Verifica superfícies brancas continuamente
  processarComandosBT(); // Processa comandos Bluetooth
  
  delay(50); // Pequeno delay para estabilidade
}