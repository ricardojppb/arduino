
#include <Servo.h>    // inclui biblioteca de manipulação de servos motores.    
#include <AFMotor.h>   // inclui biblioteca de manipulação de motores DCs.  

//Definindo os pinos sonar
#define trigPin A0 //Pino TRIG
#define echoPin A1 //Pino ECHO

//Definicao dos motores
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

int tempoGirar = 1;//esse é o tempo para o robô girar em 45º com uma bateria de 9v.
int distanciaObstaculo = 20; //distância para o robô parar e recalcular o melhor caminho
int velocidadeMotoresD = 70; // velocidade que os motores funcionarão na bateria 9v. Para a bateria 9v a velocidade 80 é ideal
int velocidadeMotoresE = 70;
int servoC = 90, servoD = 180, servoE = 0;

Servo servo_ultra_sonico; // nomeando o servo motor

//variáveis  para o sensor ultrassonico
long duracao;
long distancia_cm = 0;
int minimumRange = 5; //tempo de resposta do sensor
int maximumRange = 200;

// executado na inicialização do Arduino
void setup() {

  Serial.begin(9600); // inicializa a comunicação serial para mostrar dados

  servo_ultra_sonico.attach(10);  // Define o mini servo motor ligado no pino digital 10.
  pinMode(trigPin, OUTPUT); //define o pino TRIG como saída
  pinMode(echoPin, INPUT);  //define o pino ECHO como entrada

  motor1.setSpeed(velocidadeMotoresD);     // Define a velocidade para os motores. A velocidade máxima é 255.
  motor2.setSpeed(velocidadeMotoresE);     //Usamos uma bateria de 9v 450mAh, com ela a velocidade ficou boa. Mas dependendo da bateria utilizada a velocidade deverá ser utilizada. Não use pilhas, pois são fracas

  //inicializa com os motores parados
  motor1.run(RELEASE);
  motor2.run(RELEASE);

  servo_ultra_sonico.write(servoC);   // O servo do sensor se inicia a 90 graus (meio)

  parar();  //inica com os motores parados
}

// Função principal do Arduino
void loop() {

  pensar(); //inicia a função pensar

}

// Função para chamar outras funções e definir o que o robô fará
void pensar() {

  reposicionaServoSonar(); //Coloca o servo para olhar a frente

  int distancia = lerSonar(); // Ler o sensor de distância

  Serial.print("distancia em cm: ");
  Serial.println(distancia);   // Exibe no serial monitor

  if (distancia > distanciaObstaculo) {  // Se a distância for maior que 20 cm

    frente(); //robô anda para frente

  } else {

    parar();  //para o robô
    posicionaCarroMelhorCaminho(); //calcula o melhor caminho
    pensar();

  }
}

// Função para ler e calcular a distância do sensor ultrassônico
int lerSonar() {

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duracao = pulseIn(echoPin, HIGH); //Captura a duração em tempo do retorno do som.
  distancia_cm = duracao / 56; //Calcula a distância

  delay(10);

  return distancia_cm; // Retorna a distância
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

  Serial.print("Distancia Esquerda: ");
  Serial.println(leituraDoSonar);

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
  delay(200);
}

// Função para fazer o carro parar
void parar() {

  Serial.println(" Motor: Parar ");
  motor1.run(RELEASE); // Motor para
  motor2.run(RELEASE);

}

// Função para fazer o robô andar para frente
void frente() {

  Serial.println("Motor: Frente ");
  motor1.run(BACKWARD); // Roda vai para frente
  motor2.run(BACKWARD);
  delay(50);

}

// Função que faz o robô andar para trás e emite som quando ele dá ré
void re() {

  Serial.println("Motor: ré ");
  for (int i = 0; i <= 3; i++) {
    delay(100);
    motor1.run(FORWARD);    // Roda vai para trás
    motor2.run(FORWARD);    // Roda vai para trás
    delay(100);
  }
  parar();

}

// Função que faz o robô virar à direita, https://SeuRobo.com.br/
void direita() {

  motor1.setSpeed(velocidadeMotoresD - 10);
  motor2.setSpeed(velocidadeMotoresE - 10);

  delay(100);
  motor1.run(FORWARD);    //o robô dá uma ré para não colidir ao girar
  motor2.run(FORWARD);
  delay(50);
  Serial.println(" Para a direita ");
  motor1.run(BACKWARD);
  motor2.run(FORWARD); // Roda vai para frente
  delay(tempoGirar);

}

// Função que faz o robô virar à esquerda
void esquerda() {

  motor1.setSpeed(velocidadeMotoresD - 10);
  motor2.setSpeed(velocidadeMotoresE - 10);

  delay(100);
  motor1.run(FORWARD);    // // O robô dá uma ré para não colidir ao girar
  motor2.run(FORWARD);
  delay(50);
  Serial.println(" Para a esquerda ");
  motor1.run(FORWARD); // Roda vai para frente
  motor2.run(BACKWARD); // Roda vai para trás
  delay(tempoGirar);

}
