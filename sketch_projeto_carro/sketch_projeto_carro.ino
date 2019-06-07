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

void setup() {

  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");

}

void loop() {

  //Le as informacoes do sensor, em cm e pol
  float cmMsecCentral, inMsecCentral, cmMsecDireita,
        inMsecDireita, cmMsecEsquerda, inMsecEsquerda;

  long microsecCentral = ultrasonicoCentral.timing();
  long microsecDireita = ultrasonicoDireita.timing();
  long microsecEsquerda = ultrasonicoEsquerda.timing();

  //Lendo em centimetros
  cmMsecCentral = ultrasonicoCentral.convert(microsecCentral, Ultrasonic::CM);
  //Lendo em polegadas
  //inMsecCentral = ultrasonicoCentral.convert(microsecCentral, Ultrasonic::IN);

  //Lendo em centimetros
  cmMsecDireita = ultrasonicoDireita.convert(microsecDireita, Ultrasonic::CM);
  //Lendo em polegadas
  //inMsecDireita = ultrasonicoDireita.convert(microsecDireita, Ultrasonic::IN);

  //Lendo em centimetros
  cmMsecEsquerda = ultrasonicoEsquerda.convert(microsecEsquerda, Ultrasonic::CM);
  //Lendo em polegadas
  //inMsecEsquerda = ultrasonicoEsquerda.convert(microsecEsquerda, Ultrasonic::IN);

  Serial.print("Distancia Central em cm: ");
  Serial.println(cmMsecCentral);
  Serial.print("Distancia Direita em cm: ");
  Serial.println(cmMsecDireita);
  Serial.print("Distancia Esquerda em cm: ");
  Serial.println(cmMsecEsquerda);
  delay(1000);


}

void move_roda_direita_frente() {

  for (int i = 0; i < 255; i++) {
    m.motor(motor_direito, FORWARD, i);
    // m.motor(motor_esquerdo, FORWARD, i);
    delay(10);
  }
}

void move_roda_esqueda_frente() {

  for (int i = 0; i < 255; i++) {
    // m.motor(motor_direito, FORWARD, i);
    m.motor(motor_esquerdo, FORWARD, i);
    delay(10);
  }
}

void move_roda_direita_traz() {

  for (int i = 0; i < 255; i++) {
    m.motor(motor_direito, BACKWARD, i);
    //m.motor(motor_esquerdo, BACKWARD, i);
    delay(10);
  }
}

void move_roda_esqueda_traz() {

  for (int i = 0; i < 255; i++) {
    //m.motor(motor_direito, FORWARD, i);
    m.motor(motor_esquerdo, BACKWARD, i);
    delay(10);
  }
}

void move_frente() {
  move_roda_direita_frente();
  move_roda_esqueda_frente();
}


void move_traz() {
  move_roda_direita_traz();
  move_roda_esqueda_traz();
}
