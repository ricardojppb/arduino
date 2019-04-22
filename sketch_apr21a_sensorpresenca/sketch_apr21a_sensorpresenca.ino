#define pino_sensor 7
int acionamento;
int pino_led = 13;

int pino_led_verde = 2;
int pino_led_vermelho = 4;

//Variável para calibração do sensor
int calibracao = 45;

void setup() {

  Serial.begin(9600);

  pinMode(pino_sensor, INPUT);

  Serial.print("Calibrando o sensor...");
  for (int i = 0; i < calibracao; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Sensor Ativado");
  delay(500);

}

void loop() {
  
  acionamento = digitalRead(pino_sensor);

  Serial.print(acionamento);
  Serial.print(" ");

  if (acionamento == LOW) {

    digitalWrite(pino_led_verde, LOW);
    digitalWrite(pino_led_vermelho, HIGH);
    Serial.println("Nenhum movimento!!");

  } else {

    digitalWrite(pino_led_verde, HIGH);
    digitalWrite(pino_led_vermelho, LOW);
    Serial.println("Movimento detectado!!");
  }


}
