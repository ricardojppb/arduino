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

#define pSendor1 7
#define pSendor2 8
#define pSendor3 9
#define pSendor4 10
#define pSendor5 11

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonicoCentral(pUltrasonicoCentral_trigger, pUltrasonicoCentral_echo);
Ultrasonic ultrasonicoDireita(pUltrasonicoDireita_trigger, pUltrasonicoDireita_echo);
Ultrasonic ultrasonicoEsquerda(pUltrasonicoEsquerda_trigger, pUltrasonicoEsquerda_echo);

//Definindo a classe para controlar os motores
MotorDriver m;

int SENSORA1, SENSORA2, SENSORA3, SENSORA4, SENSORA5;

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

  SENSORA1 = analogRead(pSendor1);
  SENSORA2 = analogRead(pSendor2);
  SENSORA3 = analogRead(pSendor3);
  SENSORA4 = analogRead(pSendor4);
  SENSORA5 = analogRead(pSendor5);

  Serial.print("Sensor A7: ");
  Serial.println(SENSORA1);
  Serial.print("Sensor A8: ");
  Serial.println(SENSORA2);
  Serial.print("Sensor A9: ");
  Serial.println(SENSORA3);
  Serial.print("Sensor A10: ");
  Serial.println(SENSORA4);
  Serial.print("Sensor A11: ");
  Serial.println(SENSORA5);
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
    frente(velocidadeMax);

    if (cmMsecDireita <= distMinLateral) {

      Serial.println("Parede proxima  - Movendo para esquerda!");
      esqueda(velocidadeMedia, 60);

    } else if (cmMsecEsquerda <= distMinLateral) {

      Serial.println("Parede proxima  - Movendo para direita!");
      direita(velocidadeMedia, 60);

    }

  } else if (cmMsecDireita >= distMaxLateral) {

    Serial.println("Movendo para direita!");
    direita(velocidadeMax, 45);

  } else if (cmMsecEsquerda >= distMaxLateral) {

    Serial.println("Movendo para esquerda!");
    esqueda(velocidadeMax, 45);

  } else {

    Serial.println("Movendo para traz!");
    traz(velocidadeMedia);

  }

  //delay(1000);
}

//void regular_centralizacao_esquerda(int velocidade) {
//
//  parar();
//  //  m.motor(motor_esquerdo, FORWARD, 0);
//  //  m.motor(motor_direito, BACKWARD, 0);
//  //
//  //  delay(50);
//
//  for (int i = velocidade; i > velocidadeMin; i--) {
//
//    m.motor(motor_esquerdo, FORWARD, i);
//    if (i <= velocidadeMedia) {
//      m.motor(motor_direito, BACKWARD, i);
//    }
//
//  }
//
//}
//
//void regular_centralizacao_direita(int velocidade) {
//
//  parar();
//  //  m.motor(motor_direito, BACKWARD, 0);
//  //  m.motor(motor_esquerdo, FORWARD, 0);
//  //
//  //  delay(50);
//
//  for (int i = velocidade; i > velocidadeMin; i--) {
//
//    m.motor(motor_direito, BACKWARD, i);
//    if (i <= velocidadeMedia) {
//      m.motor(motor_esquerdo, FORWARD, i);
//    }
//
//  }
//}

void direita(int velocidade, int tempo) {

  parar();
  //  m.motor(motor_direito, BACKWARD, 0);
  //  m.motor(motor_esquerdo, FORWARD, 0);
  //
  //  delay(tempo);

  for (int i = velocidade; i > velocidadeMin; i--) {
    m.motor(motor_direito, BACKWARD, i);
    m.motor(motor_esquerdo, BACKWARD, i);
  }

  delay(tempo);
}

void esqueda(int velocidade, int tempo) {

  parar();
  //  m.motor(motor_esquerdo, FORWARD, 0);
  //  m.motor(motor_direito, BACKWARD, 0);
  //
  //  delay(tempo);

  for (int i = velocidade; i > velocidadeMin; i--) {
    m.motor(motor_esquerdo, FORWARD, i);
    m.motor(motor_direito, FORWARD, i);
  }

  delay(tempo);
}

//void move_direita_traz(int velocidade) {
//
//  //m.motor(motor_esquerdo, BACKWARD, 0);
//  //m.motor(motor_direito, FORWARD, 0);
//
//  parar();
//
//  //delay(50);
//
//  for (int i = velocidade; i > velocidadeMin; i--) {
//    m.motor(motor_direito, FORWARD, i);
//    m.motor(motor_esquerdo, FORWARD, i);
//  }
//
//  delay(50);
//
//}

//void move_esqueda_traz(int velocidade) {
//
//  //m.motor(motor_direito, FORWARD, 0);
//  //m.motor(motor_esquerdo, BACKWARD, 0);
//
//  parar();
//
//  //delay(50);
//
//  for (int i = velocidade; i > velocidadeMin; i--) {
//    m.motor(motor_direito, BACKWARD, i);
//    m.motor(motor_esquerdo, BACKWARD, i);
//  }
//
//  delay(50);
//
//}

void frente(int velocidade) {

  delay(20);

  for (int i = velocidade; i > velocidadeMin; i--) {

    m.motor(motor_direito, BACKWARD, i);
    m.motor(motor_esquerdo, FORWARD, i);

  }

  delay(10);

}

void traz(int velocidade) {

  delay(20);

  for (int i = velocidade; i > velocidadeMin; i--) {

    m.motor(motor_direito, FORWARD, i);
    m.motor(motor_esquerdo, BACKWARD, i);

  }

  delay(10);
}

void parar() {

  m.motor(motor_direito, BRAKE, 0);
  m.motor(motor_esquerdo, BRAKE, 0);
  delay(100);
}
