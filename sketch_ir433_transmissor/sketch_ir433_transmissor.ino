//#include <VirtualWire.h>
//
////Define pinos Led e Botao
//const int ledPin = 13;
//const int pino_botao = A0;
//
//int valor_botao;
//char Valor_CharMsg[4];
////Armazena estado led = ligar/desligar
//int estado = 0;
//
//void setup()
//{
//  Serial.begin(9600);
//  pinMode(ledPin,OUTPUT);
//  pinMode(pino_botao,INPUT);
//  //Pino ligado ao pino DATA do transmissor RF
//  vw_set_tx_pin(4);
//  //Velocidade de comunicacao (bits por segundo)
//  vw_setup(5000);
//  Serial.println("Trasmissao modulo RF - Acione o botao...");
//}
//
//void loop()
//{
//  //Verifica o estado do push-button
//  valor_botao = digitalRead(pino_botao);
//  //itoa(valor_botao,Valor_CharMsg,10);
//
//  //Caso o botao seja pressionado, envia dados
//  if (valor_botao == 0)
//  {
//    //Altera o estado do led
//    estado = !estado;
//    //Converte valor para envio
//    itoa(estado,Valor_CharMsg,10);
//    //Liga o led da porta 13 para indicar envio dos dados
//    digitalWrite(13, true);
//    //Envio dos dados
//    vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));
//    //Aguarda envio dos dados
//    vw_wait_tx();
//    //Desliga o led da porta 13 ao final da transmissao
//    digitalWrite(13, false);
//    Serial.print("Valor enviado: ");
//    Serial.println(Valor_CharMsg);
//    delay(500);
//  }
//}
//
//#include <VirtualWire.h>
//
//bool estado_led;
//char data[2];
//
//void setup()
//{
//  Serial.begin(9600);
//  vw_set_tx_pin(5);
//  vw_setup(2000);
//
//}
//
//void loop()
//{
//  estado_led = !estado_led;
//  itoa(estado_led, data, 2);
//  vw_send((uint8_t *)data, strlen(data));
//  vw_wait_tx();
//  Serial.println(estado_led);
//  delay(200);
//
//
//
//  }

// Código de funcionamento Transmissor RF
// Projeto de comunicação sem fio Arduino

#include <VirtualWire.h> // Inclui biblioteca necessária para comunicação

void setup() {
  
  Serial.begin(9600);      // Configura a comunicação serial em 9600

  vw_set_tx_pin(5);        // Configura o pino Digital utilizado pelo Módulo
  vw_setup(2000);          // Bits per segundo

  Serial.println("Digite o texto que deseja Enviar ..."); //Imprime na Porta Serial
}

void loop() {
  char data[40];
  int numero;

  if (Serial.available() > 0)  {
    numero = Serial.readBytesUntil (13, data, 40);
    data[numero] = 0;
    Serial.print("Enviado : ");    // Imprime na porta serial a mensagem em destaque
    Serial.println(data);          // Imprime o valor enviado

    send(data);                    // Envia o valor para o receptor
  }
}

void send (char *message) {
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx();                  // Aguarda o envio de dados
}
