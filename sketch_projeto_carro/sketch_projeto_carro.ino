#include <MotorDriver.h>
#include <Ultrasonic.h>

//Define os motores da sheild
#define motor_direito 1
#define motor_esquerdo 4

//Define os pinos para o trigger e echo
#define pino_trigger 4
#define pino_echo 5

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

//Definindo a classe para controlar os motores
MotorDriver m;

void setup() {

  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");

}

void loop() {

  //Le as informacoes do sensor, em cm e pol
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();

  //Lendo em centimetros
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  //Lendo em polegadas
  //inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  Serial.print("Distancia em cm: ");
  Serial.printnl(cmMsec);
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
