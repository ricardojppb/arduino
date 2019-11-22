//Emula a serial nos pinos digitais 4 e 5
#include <SoftwareSerial.h>

//Definindo RX, TX
SoftwareSerial SerialBT(4, 5);

//Configuracoes biblioteca Blynk
#define BLYNK_PRINT SerialBT
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleSerialBLE.h>

//definindo o pino do rele
#define pino_rele 6

//definindo o pino do sensor
const int pino_sensor = 7;
//#define pino_sensor 7

//definindo o pino do sensor
//#define pino_sensor_ativo 8
int pino_sensor_ativo = HIGH;

#define PIN_V0 V0

//Variável para calibração do sensor
int calibracao = 10;
//int detectado_movimento;

//Token de autorizacao
char auth[] = "f22ca67079e548d78a5fa8f49899af60";

BlynkTimer timer;

void setup() {
  
  SerialBT.begin(9600);
  Serial.begin(4800);

  pinMode(pino_sensor, INPUT);
  delay(5000);
  //Inicializa a comunicacao serial com o modulo HC-05
  Blynk.begin(SerialBT, auth);
  delay(500);
  timer.setInterval(1000L, movimento);

}

void loop() {

  //movimento();
  //Aguarda conexao e interpreta os comandos enviados
  Blynk.run();
  delay(1000);

}

void movimento() {

  Serial.println("Teste");
  if (pino_sensor_ativo == HIGH) {

    int detectado_movimento = digitalRead(pino_sensor);

    if (detectado_movimento == LOW) {

      digitalWrite(pino_rele, LOW);

    } else {

      digitalWrite(pino_rele, HIGH);

    }
  }
}
