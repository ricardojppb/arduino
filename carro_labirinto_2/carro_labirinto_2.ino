#include <Servo.h>    // inclui biblioteca de manipulação de servos motores.    
#include <AFMotor.h>   // inclui biblioteca de manipulação de motores DCs.  
#include <Ultrasonic.h>

//Definindo os pinos sonar
#define trigPin A0 //Pino TRIG
#define echoPin A1 //Pino ECHO

//Definindo os pinos sensor reflexivo
#define sensorD A2
#define sensorE A3

//Definicao dos motores
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

int tempoGirar = 300;//esse é o tempo para o robô girar em 45º com uma bateria de 9v.
int distanciaObstaculo = 17; //distância para o robô parar e recalcular o melhor caminho

int velocidadeMotoresDCalibracao = 4;
int velocidadeMotoresD = 65 - velocidadeMotoresDCalibracao; // velocidade que os motores funcionarão na bateria 9v. Para a bateria 9v a velocidade 80 é ideal
int velocidadeMotoresE = 65;
int velocidadeMotoresDMax = 95 - velocidadeMotoresDCalibracao; // velocidade que os motores funcionarão na bateria 9v. Para a bateria 9v a velocidade 80 é ideal
int velocidadeMotoresEMax = 95;
int servoC = 90, servoD = 180, servoE = 0;

Servo servo_ultra_sonico; // nomeando o servo motor
Ultrasonic ultrasonic(trigPin, echoPin);

//variáveis  para o sensor ultrassonico
long duracao;
long distancia_cm = 0;

// executado na inicialização do Arduino
void setup() {

  Serial.begin(9600); // inicializa a comunicação serial para mostrar dados


  pinMode(sensorD, INPUT);
  pinMode(sensorE, INPUT);

  servo_ultra_sonico.attach(10);  // Define o mini servo motor ligado no pino digital 10.
  //pinMode(trigPin, OUTPUT); //define o pino TRIG como saída
  //pinMode(echoPin, INPUT);  //define o pino ECHO como entrada

  parar();  //inica com os motores parados

  reposicionaServoSonar();   // O servo do sensor se inicia a 90 graus (meio)

  for ( int i = 0; i < 3; i++) {

    lerSonar();
  }

}

// Função principal do Arduino
void loop() {

  //  Serial.print("Sensor Direito: ");
  //  Serial.print(analogRead(sensorD));
  //  Serial.print(" -- ");
  //  Serial.print("Sensor Esquerdo: ");
  //  Serial.println(analogRead(sensorE));

  //  Serial.print("Sensor Direito: ");
  //  Serial.print(digitalRead(sensorD));
  //  Serial.print(" -- ");
  //  Serial.print("Sensor Esquerdo: ");
  //  Serial.println(digitalRead(sensorE));

  pensar(); //inicia a função pensar

}

// Função para chamar outras funções e definir o que o robô fará
void pensar() {

  reposicionaServoSonar(); //Coloca o servo para olhar a frente

  int distancia = lerSonar(); // Ler o sensor de distância

  Serial.print("distancia em cm: ");
  Serial.println(distancia);   // Exibe no serial monitor

  if (distancia > distanciaObstaculo) {  // Se a distância for maior que 20 cm

    if (distancia > 2000 or distancia < 1) {

      parar();
      re();
      posicionaCarroMelhorCaminho();
      pensar();

    } else {

      int sd = digitalRead(sensorD);
      int se = digitalRead(sensorE);

      if (sd == LOW && se == HIGH) {
        //reposicionar para esquerda
        reposicionarEsquerda();

      } else if (sd == HIGH && se == LOW) {

        //reposicionar para direita
        reposicionarDireita();

      }

      frente(); //robô anda para frente
    }

  } else {

    parar();  //para o robô
    posicionaCarroMelhorCaminho(); //calcula o melhor caminho
    pensar();

  }
}

// Função para ler e calcular a distância do sensor ultrassônico
int lerSonar() {

  distancia_cm = 0;

  for (int i = 0; i < 3; i++) {

    //    digitalWrite(trigPin, LOW);
    //
    //    delayMicroseconds(2);
    //
    //    digitalWrite(trigPin, HIGH);
    //
    //    delayMicroseconds(10);
    //
    //    digitalWrite(trigPin, LOW);
    //
    //    int distancia = 0;
    //
    //    duracao = pulseIn(echoPin, HIGH); //Captura a duração em tempo do retorno do som.
    //
    //    distancia = duracao / 56; //Calcula a distância
    //
    //    distancia_cm = distancia_cm + distancia;


    // novo teste
    float cmMsec;
    long microsec = ultrasonic.timing();
    distancia_cm = ultrasonic.convert(microsec, Ultrasonic::CM);

    delay(5);

  }

  return distancia_cm;
  //return distancia_cm / 10;
}

// Função para calcular a distância do centro
int calcularDistanciaCentro() {

  servo_ultra_sonico.write(servoC);
  int leituraDoSonar = 0;

  for (int i = 0; i < 10; i++) {

    leituraDoSonar = lerSonar();  // Ler sensor de distância
    delay(10);

    leituraDoSonar = lerSonar();
    delay(10);

  }

  Serial.print("Distancia do Centro: "); // Exibe no serial
  Serial.println(leituraDoSonar);

  if (leituraDoSonar > 2000) {
     
     re();
     leituraDoSonar = calcularDistanciaCentro();
     
  }

  return leituraDoSonar;       // Retorna a distância
}

// Função para calcular a distância da direita
int calcularDistanciaDireita() {

  servo_ultra_sonico.write(servoD);
  int leituraDoSonar = 0;

  for (int i = 0; i < 10; i++) {

    leituraDoSonar = lerSonar();
    delay(10);

    leituraDoSonar = lerSonar();
    delay(10);
  }

  Serial.print("Distancia da Direita: ");
  Serial.println(leituraDoSonar);

  if (leituraDoSonar > 2000) {

     re();
     esquerda();
     leituraDoSonar = calcularDistanciaDireita();
     
  }

  return leituraDoSonar;
}

// Função para calcular a distância da esquerda
int calcularDistanciaEsquerda() {

  servo_ultra_sonico.write(servoE);
  int leituraDoSonar = 0;

  for (int i = 0; i < 10; i++) {

    leituraDoSonar = lerSonar();
    delay(10);

    leituraDoSonar = lerSonar();
    delay(10);
  }

  Serial.print("Distancia da Esquerda: ");
  Serial.println(leituraDoSonar);

  if (leituraDoSonar > 2000) {
     
     re();
     direita();
     leituraDoSonar = calcularDistanciaEsquerda();
     
  }

  return leituraDoSonar;
}

// Função para captar as distâncias lidas e calcular a melhor distância. Acesse: Seu Robô https://SeuRobo.com.br/
char calculaMelhorDistancia() {

  int direita = calcularDistanciaDireita();
  int centro = calcularDistanciaCentro();
  int esquerda = calcularDistanciaEsquerda();

  reposicionaServoSonar();

  int maiorDistancia = 0;
  char melhorDistancia = '0';

  if (centro > direita && centro > esquerda) {

    melhorDistancia = 'c';
    maiorDistancia = centro;

  } else if (direita > centro && direita > esquerda) {

    melhorDistancia = 'd';
    maiorDistancia = direita;

  } else if (esquerda > centro && esquerda > direita) {

    melhorDistancia = 'e';
    maiorDistancia = esquerda;

  } else {
    
    re();
    calculaMelhorDistancia();
    
  }

  if (centro > 2000 or direita > 2000 or esquerda > 2000) {
      
      re();
      calculaMelhorDistancia();
  }

  if (maiorDistancia <= distanciaObstaculo) { //distância limite para parar o robô

    re();
    posicionaCarroMelhorCaminho();

  }

  reposicionaServoSonar();

  return melhorDistancia;
}

// Função para colocar o carrinho na melhor distância, isto é, girá-lo para a melhor distância
void posicionaCarroMelhorCaminho() {

  char melhorDist = calculaMelhorDistancia();

  Serial.print("melhor Distancia em cm: ");
  Serial.println(melhorDist);

  if (melhorDist == 'c') {

    pensar();

  } else if (melhorDist == 'd') {

    direita();

  } else if (melhorDist == 'e') {

    esquerda();

  } else {

    re();

  }

  reposicionaServoSonar();
}

// Função para deixar o sensor "olho" do robô no centro
void reposicionaServoSonar() {

  servo_ultra_sonico.write(servoC);
  delay(50);
}

// Função para fazer o carro parar
void parar() {

  Serial.println(" Motor: Parar ");
  motor1.setSpeed(0);
  motor2.setSpeed(0);

  delay(10);

  motor1.run(RELEASE); // Motor para
  motor2.run(RELEASE);

  delay(50);

  motor1.setSpeed(velocidadeMotoresD);
  motor2.setSpeed(velocidadeMotoresE);

}

// Função para fazer o robô andar para frente
void frente() {

  Serial.println("Motor: Frente ");

  motor1.setSpeed(velocidadeMotoresDMax);
  motor2.setSpeed(velocidadeMotoresEMax);

  delay(5);

  motor1.run(BACKWARD); // Roda vai para frente
  motor2.run(BACKWARD);

  delay(10);

  motor1.setSpeed(velocidadeMotoresD);
  motor2.setSpeed(velocidadeMotoresE);

  delay(5);

  motor1.run(BACKWARD); // Roda vai para frente
  motor2.run(BACKWARD);

  //delay(10);

}

void re() {

  Serial.println("Motor: ré ");
  motor1.setSpeed(velocidadeMotoresDMax);
  motor2.setSpeed(velocidadeMotoresEMax);

  delay(5);

  motor1.run(FORWARD);
  motor2.run(FORWARD);

  delay(150);

  parar();

}


void esquerda() {

   Serial.println("Motor: Esquerda");

  re();
  //
  //  delay(100);
  motor1.setSpeed(velocidadeMotoresDMax);
  motor2.setSpeed(velocidadeMotoresEMax);

  delay(5);

  motor1.run(BACKWARD); // Roda para trás
  motor2.run(FORWARD); // Roda vai para frente

  delay(tempoGirar);

  parar();

}

// Função que faz o robô virar à esquerda
void direita() {

   Serial.println("Motor: Direita ");

  re();

  //  delay(100);
  motor1.setSpeed(velocidadeMotoresDMax);
  motor2.setSpeed(velocidadeMotoresEMax);

  delay(5);

  motor1.run(FORWARD); // Roda vai para frente
  motor2.run(BACKWARD); // Roda vai para trás

  delay(tempoGirar);

  parar();

}

void reposicionarEsquerda() {

  Serial.println("Motor: Reposicionar para Esquerda");
   re();
  
  motor1.setSpeed(velocidadeMotoresDMax);
  motor2.setSpeed(velocidadeMotoresEMax);

  delay(5);

  motor1.run(BACKWARD); // Roda para trás
  motor2.run(FORWARD); // Roda vai para frente

  delay(50);

  parar();

}

// Função que faz o robô virar à esquerda
void reposicionarDireita() {

  Serial.println("Motor: Reposicionar para Direita");
  re();

  motor1.setSpeed(velocidadeMotoresDMax);
  motor2.setSpeed(velocidadeMotoresEMax);

  delay(5);

  motor1.run(FORWARD); // Roda vai para frente
  motor2.run(BACKWARD); // Roda vai para trás

  delay(50);

  parar();

}
