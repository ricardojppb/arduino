//#include <VirtualWire.h>
//
////Define pino led
//int ledPin = 13;
//
//int valor_recebido_RF;
//char recebido_RF_char[4];
//
//void setup()
//{
//  Serial.begin(9600);
//  pinMode(ledPin, OUTPUT);
//  //Pino ligado ao pino DATA do receptor RF
//  vw_set_rx_pin(5);
//  //Velocidade de comunicacao (bits por segundo)
//  vw_setup(2000);
//  //Inicia a recepcao
//  vw_rx_start();
//  Serial.println("Recepcao modulo RF - Aguardando...");
//}
//
//void loop()
//{
//    uint8_t buf[VW_MAX_MESSAGE_LEN];
//    uint8_t buflen = VW_MAX_MESSAGE_LEN;
//
//    if (vw_get_message(buf, &buflen))
//    {
//    int i;
//        for (i = 0; i < buflen; i++)
//       {
//          //Armazena os caracteres recebidos
//          recebido_RF_char[i] = char(buf[i]);
//       }
//       recebido_RF_char[buflen] = '\0';
//
//       //Converte o valor recebido para integer
//       valor_recebido_RF = atoi(recebido_RF_char);
//
//       //Mostra no serial monitor o valor recebido
//       Serial.print("Recebido: ");
//       Serial.print(valor_recebido_RF);
//       //Altera o estado do led conforme o numero recebido
//       if (valor_recebido_RF == 1)
//       {
//         digitalWrite(ledPin, HIGH);
//         Serial.println(" - Led aceso !");
//       }
//       if (valor_recebido_RF == 0)
//       {
//         digitalWrite(ledPin, LOW);
//         Serial.println(" - Led apagado !");
//       }
//    }
//}


//#include <VirtualWire.h>
//
//uint8_t message[VW_MAX_MESSAGE_LEN];    // Armazena as mensagens recebidas
//uint8_t msgLength = VW_MAX_MESSAGE_LEN; // Armazena o tamanho das mensagens
//
//
//void setup()   {
//  Serial.begin(9600);
//    vw_set_rx_pin(5); // Define o pino 5 do Arduino como entrada
////de dados do receptor
//    vw_setup(2000);             // Bits por segundo
//    pinMode(13,OUTPUT);
//    digitalWrite(13, LOW);
//    vw_rx_start();              // Inicializa o receptor
//
//
//}
//
//void loop()
//{
//    //message = 'ricardo';
//    if (vw_get_message(message, &msgLength)) // Non-blocking
//    {
//        Serial.print("Recebido: ");
//        //Serial.println(message[0]);
//
//        for(int i = 0; i < sizeof(message); i++) {
//
//          Serial.print(message[i]);
//
//        }
//        Serial.println(" ");
//        if(message[0] == '0'){
//          digitalWrite(13, LOW);
//        }
//        else if(message[0] == '1'){
//          digitalWrite(13,HIGH);
//        }
//    }
//}

// Código de funcionamento Receptor RF
// Projeto de comunicação sem fio Arduino

// Inclui biblioteca necessária para comunicação
#include <VirtualWire.h>          
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
#include "string.h"

//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte message[100];    // Variável para armazenagem de mensagens

byte messageLength = 100;    // Tamanho da mensagem

void setup() {
  Serial.begin(9600);         // Configura a comunicação serial em 9600
  
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);

  Serial.println("Dispositivo pronto para receber dados!"); //Imprime na Porta Serial

  vw_set_rx_pin(7);         // Configura o pino Digital utilizado pelo Módulo
  vw_setup(2000);            // Bits por segundo
  vw_rx_start();             // Inicializa o receptor

   lcd.clear();
  //Posiciona o cursor na coluna 3, linha 0;
  lcd.setCursor(3, 0);
  //Envia o texto entre aspas para o LCD
  lcd.print("teste");
  lcd.setCursor(3, 1);
  lcd.print(" LCD 16x2");
  delay(5000);
   
  //Rolagem para a esquerda
  for (int posicao = 0; posicao < 3; posicao++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }
   
  //Rolagem para a direita
  for (int posicao = 0; posicao < 6; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(300);
  }
}

void loop() {

  String mensagem = "";
  if (vw_get_message(message, &messageLength)) {     // Elimina o bloqueio

    // Imprime na porta serial a mensagem em destaque
    Serial.print("Recebido: ");      
    //Serial.write(message[0]);
    //array_to_string(message,sizeof(message), mensagem);
    //strcpy(mensagem, (char*)message);
    for (int i = 0; i < sizeof(message); i++) {
//      
//      mensagem = mensagem + (char)message[i];
        //Serial.write(message[i]);
        mensagem += String((char)message[i]);
//      
    }
    
    Serial.print(mensagem);
    Serial.println();
    
    lcdPrint(mensagem);
    
  }
}

void lcdPrint(String messagen) {
  
  //Limpa a tela
  lcd.clear();
  //Posiciona o cursor na coluna 3, linha 0;
  lcd.setCursor(0, 0);
  //Envia o texto entre aspas para o LCD
  lcd.print("teste");
  lcd.setCursor(0, 1);
  lcd.print(" LCD 16x2");
  delay(5000);
   
  //Rolagem para a esquerda
  for (int posicao = 0; posicao < 3; posicao++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }
   
  //Rolagem para a direita
  for (int posicao = 0; posicao < 6; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(300);
  }
}
