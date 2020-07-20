
#include <Servo.h>     // Adiciona a biblitoeca Servo

//Definindo os pinos
#define trigPin A0     //Pino TRIG do sensor no pino analógico A0
#define echoPin A1     //Pino ECHO do sensor no pino analógico A1
// motor um            // Ligação dos pinos da Ponte H L298N
#define enA  11        //pino enA na porta digital 10
#define in1  9         //pino in1 na porta digital 9
#define in2  8         //pino in2 na porta digital 8
// motor dois          // Ligação dos pinos da Ponte H L298N
#define enB  5         //pino enB na porta digital 5
#define in3  7         //pino in3 na porta digital 7
#define in4  6         //pino in4 na porta digital 6
#define sensorD 3
#define sensorE 4
#define sensorF 12
 char ble;


Servo servoSensor;       // Crie um objeto Servo para controlar o Servo.

//função para procurar obtasculo a todo o tempo
int Procurar (void) {
  float duracao = 0.0;              // variavael para quartar a duração do retorno do som
  float CM = 0.0;                   // variavael para quartar a distancia
  digitalWrite(trigPin, LOW);       //não envia som
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);      //envia som
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);       //não envia o som e espera o retorno do som enviado
  duracao = pulseIn(echoPin, HIGH); //Captura a duração em tempo do retorno do som.
  CM = (duracao / 58.8);            //Calcula a distância em centimetros
  Serial.print("Distancia em CM: ");
  Serial.println(CM);               //Imprimi no monitor serial a distancia
  return CM;                        // Return to CM.
}

//Variaveis
int DistanciaDireita, DistanciaEsquerda;  // variavel de Distâncias de ambos os lados
float Distancia = 0.00;                   // variavel para guardar a distancia

//Velocidades dos motores (você pode calibrar cada motor colocando os valores de 0 a 254)
int velocidadeMotorUm =160;
int velocidadeMotorDois = 100;

// Função que é executado na inicialização do Arduino
void setup() {
  Serial.begin(9600); // inicializa a comunicação serial para mostrar dados
   
   
    
  
  
  //Define o servo na porta 13
  servoSensor.attach(13); 
  
  // Definir todos os pinos de controle do motor como saídas
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode( sensorD, INPUT_PULLUP);
  pinMode( sensorE, INPUT_PULLUP);
  pinMode( sensorF, INPUT_PULLUP);

  //Configuraçõs do sensor ultrassonico
  pinMode(trigPin, OUTPUT);     //define o pino TRIG como saída
  pinMode(echoPin, INPUT);      //define o pino ECHO como entrada
}

// Função principal do Arduino
void loop() {
 
 
  ble=Serial.read();

 if(ble=='R'){
  
  Parar ();                                         // O robô Para
  ParaTras ();                                      // O robô vai para tras
  Parar ();                                         // O robô Para
 }
 if(ble=='D'){
  Vireadireita();
 }
 if(ble=='E'){
  VireaEsquerda ();
 }
 if(ble=='F'){
  Frente();
 }
  servoSensor.write (90);                           // Gira o Servo com o sensor a 90 graus
  delay (200);                                      // Aguarda 100 milesugodos
  Distancia = Procurar ();                          // Medindo a Distancia em CM.
  if (Distancia < 10) {                             // Se há obstáculo encontrado a menos de 40cm.
   direcao ();                                      // Se Frente estiver bloqueado, mude de direção
  }
  else if (Distancia >= 40)  {                      // Se o obstáculo for encontrado entre a mais de 40cm 
    Frente ();                                      // Robô se move para a direção da Frente.
  }

  if( digitalRead(sensorE)==LOW){                   // Se lateral esquerda estiver bloqueado, mude de direção
    VireaEsquerda();
  }
   if( digitalRead(sensorD)==LOW){                  // Se lateral direita estiver bloqueado, mude de direção
    Vireadireita();
   }
 if( digitalRead(sensorF)==HIGH){                   // Se Frente estiver bloqueado, mude de direção
    direcao();
  
   }
  
 //Seu Robô http://SeuRobo.com.br/
}

// Função para pegar as distancias de cada direção
void direcao () {        
  Parar ();                                         // O robô Para
  ParaTras ();                                      // O robô vai para tras
  Parar ();                                         // O robô Para
  servoSensor.write (130);                          // Gira o Servo com o sensor a 180 graus
  delay (300);              
  DistanciaEsquerda = Procurar ();                  // Defina a Distancia da Esquerda 
  delay (300);               
  servoSensor.write (50);                            // Gira o Servo com o sensor a 0 graus
  delay (300);               
  DistanciaDireita = Procurar ();                   // Defina a Distancia da Direita
  delay (300);               
  servoSensor.write (90);                           // Gira o Servo com o sensor a 90 graus
  delay (300);              
  CompareDistance ();                               // Encontre a distância mais longa.
}

// Função para calcular qual a distancia é melhor para o robô ir
void CompareDistance () {                   
  if (DistanciaDireita > DistanciaEsquerda) {       // Se a direita está menos obstruída.
    Vireadireita ();                                // O robô vai virar a direita 
  }
  else if (DistanciaEsquerda > DistanciaDireita) {  // Se Esquerda estiver menos obstruída.
    VireaEsquerda ();                               // Robô Vire na direção esquerda.
  }
  else {                                            // Se ambos estiverem igualmente obstruídos.
    Retorne ();                                     // Robô Vire-se.
  }
}

// Função para fazer o carro parar
void Parar()
{
  Serial.println("Robô: Parar ");
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(50);                                       //aguarda um tempo
}

// Função para fazer o robô andar para frente
void Frente()
{
  Serial.println("Robô: Frente ");
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

// Função que faz o robô andar para trás e emite som quando ele dá ré
void ParaTras()
{
  Serial.println("Robô: Ré ");
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(500);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

// Função que faz o robô virar à direita, https://SeuRobo.com.br/
void Vireadireita()
{
  Serial.println("Robô: Direita ");
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(500);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

// Função que faz o robô virar à esquerda
void VireaEsquerda()
{
  Serial.println("Robô: Esquerda ");                
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(500);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

//Função para o robô virar para o outro lado
void Retorne () {    
  Serial.println("Robô: Girar ");      
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay (3000);                                      //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}
