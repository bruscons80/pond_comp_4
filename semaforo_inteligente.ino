
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

bool carroEsperando = false;

void setup() {
  Serial.begin(9600); 


  pinMode(S1_VERMELHO, OUTPUT);
  pinMode(S1_VERDE, OUTPUT);
  pinMode(S2_VERMELHO, OUTPUT);
  pinMode(S2_AMARELO, OUTPUT);
  pinMode(S2_VERDE, OUTPUT);

  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);

  
  controlarSemaforo1('V'); 
  controlarSemaforo2('R'); 
}

void loop() {
  controlarSemaforo1('V');
  controlarSemaforo2('R');
  
  carroEsperando = false; 
  long tempoInicio = millis();
  while (millis() - tempoInicio < TEMPO_S1_VERDE) {
    if (lerDistancia() < DISTANCIA_DETECCAO) {
      carroEsperando = true;
    }
  }

  controlarSemaforo1('A'); // Amarelo
  controlarSemaforo2('R');
  delay(TEMPO_S1_AMARELO);

  controlarSemaforo1('R');
  controlarSemaforo2('R');
  delay(TEMPO_SEGURANCA);

  if (carroEsperando) {
    controlarSemaforo1('R');
    controlarSemaforo2('V'); 
    delay(TEMPO_S2_VERDE);
  } else {

  }

  
  if (carroEsperando) {
    controlarSemaforo1('R');
    controlarSemaforo2('A'); 
    delay(TEMPO_S2_AMARELO);
  }

  controlarSemaforo1('R');
  controlarSemaforo2('R');
  delay(TEMPO_SEGURANCA);

}


long lerDistancia() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);


  long duracao = pulseIn(PINO_ECHO, HIGH);

  long distancia = (duracao * 0.0343) / 2;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  return distancia;
}

void controlarSemaforo1(char cor) {
  if (cor == 'R') {
    digitalWrite(S1_VERMELHO, HIGH);
    digitalWrite(S1_VERDE, LOW);
  } else if (cor == 'V') {
    digitalWrite(S1_VERMELHO, LOW);
    digitalWrite(S1_VERDE, HIGH);
  } else if (cor == 'A') {
    digitalWrite(S1_VERMELHO, HIGH); // Amarelo = Vermelho + Verde
    digitalWrite(S1_VERDE, HIGH);
  } else { // Desliga
    digitalWrite(S1_VERMELHO, LOW);
    digitalWrite(S1_VERDE, LOW);
  }
}


void controlarSemaforo2(char cor) {
  if (cor == 'R') {
    digitalWrite(S2_VERMELHO, HIGH);
    digitalWrite(S2_VERDE, LOW);
    digitalWrite(S2_AMARELO, LOW);
  } else if (cor == 'V') {
    digitalWrite(S2_VERMELHO, LOW);
    digitalWrite(S2_VERDE, HIGH);
    digitalWrite(S2_AMARELO, LOW);
  } else if (cor == 'A') {
    digitalWrite(S2_VERMELHO, LOW);
    digitalWrite(S2_VERDE, LOW);
    digitalWrite(S2_AMARELO, HIGH);
  } else { // Desliga
    digitalWrite(S2_VERMELHO, LOW);
    digitalWrite(S2_VERDE, LOW);
    digitalWrite(S2_AMARELO, LOW);
  }
}