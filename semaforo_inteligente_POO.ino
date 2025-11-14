class Semaforo {
private:
  int pinVermelho;
  int pinAmarelo;
  int pinVerde;

public:
  Semaforo(int pinR, int pinY, int pinG) {
    pinVermelho = pinR;
    pinAmarelo = pinY;
    pinVerde = pinG;
  }

  Semaforo(int pinR, int pinG) {
    pinVermelho = pinR;
    pinAmarelo = -1; 
    pinVerde = pinG;
  }

  void init() {
    pinMode(pinVermelho, OUTPUT);
    pinMode(pinVerde, OUTPUT);
    if (pinAmarelo != -1) {
      pinMode(pinAmarelo, OUTPUT);
    }
  }

  void vermelho() {
    digitalWrite(pinVermelho, HIGH);
    digitalWrite(pinVerde, LOW);
    if (pinAmarelo != -1) {
      digitalWrite(pinAmarelo, LOW);
    }
  }

  void verde() {
    digitalWrite(pinVermelho, LOW);
    digitalWrite(pinVerde, HIGH);
    if (pinAmarelo != -1) {
      digitalWrite(pinAmarelo, LOW);
    }
  }

  void amarelo() {
    if (pinAmarelo != -1) {
      digitalWrite(pinVermelho, LOW);
      digitalWrite(pinVerde, LOW);
      digitalWrite(pinAmarelo, HIGH);
    } else {
      digitalWrite(pinVermelho, HIGH);
      digitalWrite(pinVerde, HIGH);
    }
  }

  void desligar() {
    digitalWrite(pinVermelho, LOW);
    digitalWrite(pinVerde, LOW);
    if (pinAmarelo != -1) {
      digitalWrite(pinAmarelo, LOW);
    }
  }
};

class SensorUltrassonico {
private:
  int pinTrig;
  int pinEcho;

public:
  SensorUltrassonico(int trig, int echo) {
    pinTrig = trig;
    pinEcho = echo;
  }

  void init() {
    pinMode(pinTrig, OUTPUT);
    pinMode(pinEcho, INPUT);
  }

  long lerDistancia() {
    digitalWrite(pinTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrig, LOW);

    long duracao = pulseIn(pinEcho, HIGH);
    long distancia = (duracao * 0.0343) / 2;
    return distancia;
  }
};

const int PINO_TRIG = 7;
const int PINO_ECHO = 6;

const int S1_VERMELHO = 9;
const int S1_VERDE = 10;

const int S2_VERMELHO = 5;
const int S2_AMARELO = 4;
const int S2_VERDE = 3;

const int DISTANCIA_DETECCAO = 30; 
const long TEMPO_S1_VERDE = 5000;
const long TEMPO_S1_AMARELO = 1500;
const long TEMPO_S2_VERDE = 5000;
const long TEMPO_S2_AMARELO = 1500;
const long TEMPO_SEGURANCA = 1000; 

Semaforo s1(S1_VERMELHO, S1_VERDE);
Semaforo s2(S2_VERMELHO, S2_AMARELO, S2_VERDE);
SensorUltrassonico sensor(PINO_TRIG, PINO_ECHO);

void setup() {
  Serial.begin(9600);

  s1.init();
  s2.init();
  sensor.init();

  s1.verde();
  s2.vermelho();
}

void loop() {
  s1.verde();
  s2.vermelho();

  bool carroEsperando = false;
  long tempoInicio = millis();

  while (millis() - tempoInicio < TEMPO_S1_VERDE) {
    long dist = sensor.lerDistancia();

    if (dist < DISTANCIA_DETECCAO) {
      carroEsperando = true;
    }

    Serial.print("Distancia: ");
    Serial.print(dist);
    Serial.println(" cm");
    
    delay(100); 
  }

  s1.amarelo();
  s2.vermelho();
  delay(TEMPO_S1_AMARELO);

  s1.vermelho();
  s2.vermelho();
  delay(TEMPO_SEGURANCA);

  if (carroEsperando) {
    s1.vermelho();
    s2.verde();
    delay(TEMPO_S2_VERDE);

    s1.vermelho();
    s2.amarelo();
    delay(TEMPO_S2_AMARELO);
  } else {
  }

  s1.vermelho();
  s2.vermelho();
  delay(TEMPO_SEGURANCA);

}