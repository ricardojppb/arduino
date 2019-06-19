#include <MotorDriver.h>
#include <BlynkSimpleSerialBLE.h>
#include  <NewPing.h>

#define BLYNK_PRINT Serial

//Define os motores da sheild
#define motor_direito 1
#define motor_esquerdo 4

//Define os pinos para o trigger e echo
#define pSC_t 22
#define pSC_e 23

#define pSD_t 24
#define pSD_e 25

#define pSE_t 26
#define pSE_e 27

#define pSendor1 7
#define pSendor2 8
#define pSendor3 9
#define pSendor4 10
#define pSendor5 11

#define MAX_DISTANCE 200
//Inicializa o sensor nos pinos definidos acima
NewPing sonar_c(pSC_t, pSC_e, MAX_DISTANCE);
NewPing sonar_d(pSD_t, pSD_e, MAX_DISTANCE);
NewPing sonar_e(pSE_t, pSE_e, MAX_DISTANCE);

//Definindo a classe para controlar os motores
MotorDriver m;
BlynkTimer timer;

char auth[] = "bed483409c46458b9d79cc3362521d93";

int SENSORA1, SENSORA2, SENSORA3, SENSORA4, SENSORA5;

const float distMinCentral = 15.0;
const float distMaxLateral = 40.0;
const float distMinLateral = 6.0;

const int velocidadeMax = 255;
const int velocidadeMedia = 200;
const int velocidadeMin = 80;

float cmMsecCentral, cmMsecDireita, cmMsecEsquerda;
long microsecCentral, microsecDireita, microsecEsquerda;
boolean isSegmento, isControle, isAutonomo;

int carregando = 10;


int linha = 500;

BLYNK_WRITE(V0) {

  switch (param.asInt())
  {
    case 1: {

        Serial.println("Funcao autonoma escolhida!");
        isAutonomo = true;
        isSegmento = false;
        isControle = false;

        break;
      }
    case 2: {

        Serial.println("Funcao segmento escolhida!");
        isSegmento = true;
        isAutonomo = false;
        isControle = false;

        break;
      }
    case 3: {

        Serial.println("Funcao controle escolhida!");
        isControle = true;
        isAutonomo = false;
        isSegmento = false;

        break;
      }
    default:
      isSegmento = isControle = isAutonomo = false;
      Serial.println("Carro desligado!");

      break;
  }
}

void calibrar() {

  isSegmento = isControle = isAutonomo = false;

  Serial.println("Conectando ao bluetooth");

  for (int i = 0; i < carregando; i++) {

    delay(500);

    //Serial.print(".");
    Serial.println(" ");

    SENSORA1 = analogRead(pSendor1); // sensor direito
    SENSORA2 = analogRead(pSendor2); // sensor direito
    SENSORA3 = analogRead(pSendor3); // sensor centro
    SENSORA4 = analogRead(pSendor4); // sensor esquerdo
    SENSORA5 = analogRead(pSendor5); // sensor esquerdo

    cmMsecCentral = sonar_c.ping_cm();
    cmMsecDireita = sonar_d.ping_cm();
    cmMsecEsquerda = sonar_e.ping_cm();

    Serial.println("**********************SONAR**************************");

    Serial.print("Distancia Direita em cm: ");
    Serial.println(cmMsecDireita);

    Serial.print("Distancia Central em cm: ");
    Serial.println(cmMsecCentral);

    Serial.print("Distancia Esquerda em cm: ");
    Serial.println(cmMsecEsquerda);

    Serial.println("****************************************************");

    delay(500);

  }

}

void setup() {

  digitalWrite(28, LOW);

  Serial.begin(9600);
  Serial3.begin(9600);

  calibrar();

  Blynk.begin(Serial3, auth);

  Serial.println("");
  Serial.println("Bluetooth conectado!");

  digitalWrite(28, HIGH);

  timer.setInterval(1L, autonomo);
  timer.setInterval(1L, controle);
  timer.setInterval(1L, segmento);

}

void loop() {

  //autonomo();
  //delay(1000);
  Blynk.run();
  timer.run();

}

void segmento() {

  if (isSegmento) {

    Serial.println("Segmento");

    SENSORA1 = analogRead(pSendor1); // sensor direito
    Serial.print("Sensor direito 2: ");
    Serial.println(SENSORA1);

    SENSORA2 = analogRead(pSendor2); // sensor direito
    Serial.print("Sensor direito 1: ");
    Serial.println(SENSORA2);

    SENSORA3 = analogRead(pSendor3); // sensor centro
    Serial.print("Sensor centro: ");
    Serial.println(SENSORA3);

    SENSORA4 = analogRead(pSendor4); // sensor esquerdo
    Serial.print("Sensor esquerdo 1: ");
    Serial.println(SENSORA4);

    SENSORA5 = analogRead(pSendor5); // sensor esquerdo
    Serial.print("Sensor esquerdo 2: ");
    Serial.println(SENSORA5);

    if (SENSORA2 < linha && SENSORA4 < linha) {
      // anda para frente
      frente(velocidadeMax, 50);
      //parar(50);
      Serial.println("Segmento para frente!!");
    }

    if (SENSORA2 > linha && SENSORA4 < linha) {
      // anda para esquerda

      direita(velocidadeMax, 10);
      frente(velocidadeMax, 50);
      //parar(50);
      Serial.println("Segmento para esquerda!!");
    }

    if (SENSORA2 < linha && SENSORA4 > linha) {
      // anda para direita
      esqueda(velocidadeMax, 10);
      frente(velocidadeMax, 50);
      //parar(50);
      Serial.println("Segmento para direita!!");
    }

    if (SENSORA2 > linha && SENSORA4 > linha) {

      // anda para frente
      esqueda(velocidadeMax, 50);
      //parar(50);
      Serial.println("Segmento para frente!!");
    }

    //    if (SENSORA1 > linha && SENSORA2 > linha && SENSORA3 > linha && SENSORA4 > linha && SENSORA5 > linha) {
    //
    //      parar(500);
    //      esqueda(velocidadeMedia, 50);
    //      Serial.println("Segmento para parado!!");
    //
    //    }

    //delay(500);
  }

}

void controle() {

  if (isControle) {

    Serial.println("Controle");

    if (digitalRead(31) == HIGH) {

      frente(velocidadeMedia, 10);

    } else if (digitalRead(32) == HIGH) {

      direita(velocidadeMedia, 10);

    } else if (digitalRead(33) == HIGH) {

      esqueda(velocidadeMedia, 10);

    } else if (digitalRead(34) == HIGH) {

      traz(velocidadeMedia, 10);

    } else {

      parar(100);

    }

  }

}


void autonomo() {

  if (isAutonomo) {

    Serial.println("Modo Autonomo Ativo.");

    cmMsecCentral = sonar_c.ping_cm();
    cmMsecDireita = sonar_d.ping_cm();
    cmMsecEsquerda = sonar_e.ping_cm();

    Serial.println("**********************SONAR**************************");

    Serial.print("Distancia Direita em cm: ");
    Serial.println(cmMsecDireita);

    Serial.print("Distancia Central em cm: ");
    Serial.println(cmMsecCentral);

    Serial.print("Distancia Esquerda em cm: ");
    Serial.println(cmMsecEsquerda);
    Serial.println("===================================================");


    if (cmMsecCentral >= distMinCentral) {

      Serial.println("Movendo para frente!");
      frente(velocidadeMax, 400);

      if (cmMsecDireita <= distMinLateral) {

        Serial.println("Parede proxima  - Movendo para direita!");
        traz(velocidadeMax, 100);
        for (int i = 0; i < 10; i++) {
          direita(velocidadeMax, 50);
        }
        frente(velocidadeMax, 100);
        //parar(5);

      } else if (cmMsecEsquerda <= distMinLateral) {

        Serial.println("Parede proxima  - Movendo para esquerda!");

        traz(velocidadeMax, 100);
        for (int i = 0; i < 10; i++) {
          esqueda(velocidadeMax, 50);
        }
        frente(velocidadeMax, 100);
        //parar(5);

      }

    } else if (cmMsecDireita >= distMaxLateral) {

      Serial.println("Movendo para direita!");
      //traz(velocidadeMax, 100);
      for (int i = 0; i < 5; i++) {
      esqueda(velocidadeMax, 50);
      }

    } else if (cmMsecEsquerda >= distMaxLateral) {

      Serial.println("Movendo para esquerda!");
      //traz(velocidadeMax, 100);
      for (int i = 0; i < 5; i++) {
        direita(velocidadeMax, 50);
      }

    } else {

      Serial.println("Movendo para traz!");
      parar(20);
      traz(velocidadeMedia, 100);

    }

    // testar esse codigo
    //    if (cmMsecCentral >= distMinCentral) {
    //
    //      Serial.println("Movendo para frente!");
    //      frente(velocidadeMax, 10);
    //
    //    } else {
    //
    //      if (cmMsecDireita > cmMsecEsquerda) {
    //
    //        Serial.println("Movendo para direita!");
    //        //traz(velocidadeMin);
    //        parar(100);
    //        direita(velocidadeMax, 350);
    //
    //      } else  if (cmMsecEsquerda  > cmMsecDireita) {
    //
    //        Serial.println("Movendo para esquerda!");
    //        //traz(velocidadeMin);
    //        parar(100);
    //        esqueda(velocidadeMax, 350);
    //
    //      } else {
    //
    //        Serial.println("Movendo para traz!");
    //        parar(100);
    //        traz(velocidadeMax, 10);
    //
    //      }

    //    }

    //delay(1000);
  }
}



void direita(int velocidade, int tempo) {

  for (int i = velocidade; i > (velocidadeMin - 10); i--) {

    m.motor(motor_esquerdo, FORWARD, i);
    m.motor(motor_direito, BACKWARD, i);

  }
  delay(tempo);

}

void esqueda(int velocidade, int tempo) {

  for (int i = velocidade; i > (velocidadeMin - 10); i--) {

    m.motor(motor_esquerdo, BACKWARD, i);
    m.motor(motor_direito, FORWARD, i);

  }

  delay(tempo);
}

void frente(int velocidade, int tempo) {

  //parar(50);
  for (int i = velocidade; i > (velocidadeMin - 10); i--) {

    m.motor(motor_esquerdo, FORWARD, i);
    m.motor(motor_direito, FORWARD, i);

  }

  delay(tempo);

}

void traz(int velocidade, int tempo) {

  //parar(50);
  for (int i = velocidade; i > (velocidadeMin - 10); i--) {

    m.motor(motor_direito, BACKWARD, i);
    m.motor(motor_esquerdo, BACKWARD, i);

  }

  delay(tempo);
}

void parar(int tempo) {

  m.motor(motor_direito, RELEASE, -1);
  m.motor(motor_esquerdo, RELEASE, -1);
  delay(tempo);
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
