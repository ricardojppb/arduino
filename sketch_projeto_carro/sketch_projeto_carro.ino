#include <MotorDriver.h>
#include <Ultrasonic.h>

//Define os motores da sheild
#define motor_direito 1
#define motor_esquerdo 4

//Define os pinos para o trigger e echo
#define pUltrasonicoCentral_trigger 22
#define pUltrasonicoCentral_echo 23

#define pUltrasonicoDireita_trigger 24
#define pUltrasonicoDireita_echo 25

#define pUltrasonicoEsquerda_trigger 26
#define pUltrasonicoEsquerda_echo 27

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonicoCentral(pUltrasonicoCentral_trigger, pUltrasonicoCentral_echo);
Ultrasonic ultrasonicoDireita(pUltrasonicoDireita_trigger, pUltrasonicoDireita_echo);
Ultrasonic ultrasonicoEsquerda(pUltrasonicoEsquerda_trigger, pUltrasonicoEsquerda_echo);

//Definindo a classe para controlar os motores
MotorDriver m;

int SENSORA7, SENSORA8, SENSORA9, SENSORA10, SENSORA11;

const float distMinCentral = 35.0;
const float distMaxLateral = 25.0;
const float distMinLateral = 5.0;



const int velocidadeMax = 255;
const int velocidadeMedia = 125;
const int velocidadeMin = 70;

void setup() {

  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");

}

void loop() {

  SENSORA7 = analogRead(7);
  SENSORA8 = analogRead(8);
  SENSORA9 = analogRead(9);
  SENSORA10 = analogRead(10);
  SENSORA11 = analogRead(11);

  Serial.print("Sensor A7: ");
  Serial.println(SENSORA7);
  Serial.print("Sensor A8: ");
  Serial.println(SENSORA8);
  Serial.print("Sensor A9: ");
  Serial.println(SENSORA9);
  Serial.print("Sensor A10: ");
  Serial.println(SENSORA10);
  Serial.print("Sensor A11: ");
  Serial.println(SENSORA11);
  Serial.println("===================================================");


  autonomo();

}

void autonomo() {

  //Le as informacoes do sensor, em cm e pol
  float cmMsecCentral, inMsecCentral, cmMsecDireita,
        inMsecDireita, cmMsecEsquerda, inMsecEsquerda;

  long microsecCentral = ultrasonicoCentral.timing();
  long microsecDireita = ultrasonicoDireita.timing();
  long microsecEsquerda = ultrasonicoEsquerda.timing();

  //Lendo em centimetros
  cmMsecDireita = ultrasonicoDireita.convert(microsecDireita, Ultrasonic::CM);
  //Lendo em polegadas
  //inMsecDireita = ultrasonicoDireita.convert(microsecDireita, Ultrasonic::IN);

  //Lendo em centimetros
  cmMsecCentral = ultrasonicoCentral.convert(microsecCentral, Ultrasonic::CM);
  //Lendo em polegadas
  //inMsecCentral = ultrasonicoCentral.convert(microsecCentral, Ultrasonic::IN);

  //Lendo em centimetros
  cmMsecEsquerda = ultrasonicoEsquerda.convert(microsecEsquerda, Ultrasonic::CM);
  //Lendo em polegadas
  //inMsecEsquerda = ultrasonicoEsquerda.convert(microsecEsquerda, Ultrasonic::IN);


  Serial.print("Distancia Direita em cm: ");
  Serial.println(cmMsecDireita);
  Serial.print("Distancia Central em cm: ");
  Serial.println(cmMsecCentral);

  Serial.print("Distancia Esquerda em cm: ");
  Serial.println(cmMsecEsquerda);
  Serial.println("===================================================");


  if (cmMsecCentral >= distMinCentral) {

    Serial.println("Movendo para frente!");
    move_frente(velocidadeMax);

    if (cmMsecDireita <= distMinLateral) {

      Serial.println("Parede proxima  - Movendo para esquerda!");
      //regular_centralizacao_esquerda(velocidadeMax);
      move_roda_esqueda_frente(velocidadeMedia, 60);

    } else if (cmMsecEsquerda <= distMinLateral) {

      Serial.println("Parede proxima  - Movendo para direita!");
      //regular_centralizacao_direita(velocidadeMax);
       move_roda_direita_frente(velocidadeMedia, 60);

    }

  } else if (cmMsecDireita >= distMaxLateral) {

    Serial.println("Movendo para direita!");
    move_roda_direita_frente(velocidadeMax, 45);
    //regular_centralizacao_direita(velocidadeMax);

  } else if (cmMsecEsquerda >= distMaxLateral) {

    Serial.println("Movendo para esquerda!");
    move_roda_esqueda_frente(velocidadeMax, 45);
    //regular_centralizacao_esquerda(velocidadeMax);

  } else {

    Serial.println("Movendo para traz!");
    move_traz(velocidadeMedia);
    
  }

  //delay(1000);
}

void regular_centralizacao_esquerda(int velocidade) {

  m.motor(motor_esquerdo, FORWARD, 0);
  m.motor(motor_direito, BACKWARD, 0);

  delay(50);

  for (int i = velocidade; i > velocidadeMin; i--) {

    m.motor(motor_esquerdo, FORWARD, i);
    if (i <= velocidadeMedia) {
      m.motor(motor_direito, BACKWARD, i);
    }

  }

}

void regular_centralizacao_direita(int velocidade) {

  m.motor(motor_direito, BACKWARD, 0);
  m.motor(motor_esquerdo, FORWARD, 0);

  delay(50);

  for (int i = velocidade; i > velocidadeMin; i--) {

    m.motor(motor_direito, BACKWARD, i);
    if (i <= velocidadeMedia) {
      m.motor(motor_esquerdo, FORWARD, i);
    }

  }
}

void move_roda_direita_frente(int velocidade, int tempo) {

  m.motor(motor_direito, BACKWARD, 0);
  m.motor(motor_esquerdo, FORWARD, 0);

  delay(tempo);

  for (int i = velocidade; i > velocidadeMin; i--) {
    m.motor(motor_direito, BACKWARD, i);
  }

   delay(tempo);
}

void move_roda_esqueda_frente(int velocidade, int tempo) {

  m.motor(motor_esquerdo, FORWARD, 0);
  m.motor(motor_direito, BACKWARD, 0);

  delay(tempo);

  for (int i = velocidade; i > velocidadeMin; i--) {
    m.motor(motor_esquerdo, FORWARD, i);
  }

  delay(tempo);
}

void move_roda_direita_traz(int velocidade) {

  m.motor(motor_esquerdo, BACKWARD, 0);
  m.motor(motor_direito, FORWARD, 0);

  delay(50);

  for (int i = velocidade; i > velocidadeMin; i--) {
    m.motor(motor_direito, FORWARD, i);
  }
  
   delay(50);

}

void move_roda_esqueda_traz(int velocidade) {

  m.motor(motor_direito, FORWARD, 0);
  m.motor(motor_esquerdo, BACKWARD, 0);

  delay(50);

  for (int i = velocidade; i > velocidadeMin; i--) {
    m.motor(motor_esquerdo, BACKWARD, i);
  }

   delay(50);

}

void move_frente(int velocidade) {

  delay(20);
  
  for (int i = velocidade; i > velocidadeMin; i--) {

    m.motor(motor_direito, BACKWARD, i);
    m.motor(motor_esquerdo, FORWARD, i);

  }

  delay(10);
  
}

void move_traz(int velocidade) {
  
  delay(20);
  
  for (int i = velocidade; i > velocidadeMin; i--) {

    m.motor(motor_direito, FORWARD, i);
    m.motor(motor_esquerdo, BACKWARD, i);

  }
  
  delay(10);
}
