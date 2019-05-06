//Emula a serial nos pinos digitais 4 e 5
#include <SoftwareSerial.h>

//Definindo RX, TX
SoftwareSerial SerialBT(4, 5);

//Configuracoes biblioteca Blynk
#define BLYNK_PRINT SerialBT
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleSerialBLE.h>

//Token de autorizacao
char auth[] = "f22ca67079e548d78a5fa8f49899af60";

void setup() {

  //Inicializa a comunicacao serial com o modulo HC-05
  SerialBT.begin(9600);
  Blynk.begin(SerialBT, auth);

}

void loop() {

  //Aguarda conexao e interpreta os comandos enviados
  Blynk.run();
  
}
