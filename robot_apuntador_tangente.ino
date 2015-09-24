#include <CustomStepper.h>
#include <math.h>
//definir sensores
//Ejes Y
#define trisen1 2
#define echosen1 3
//ejes X
#define trisen2 4
#define echosen2 5
#define laser 6
// declara Variables
double radio,R, Yb,Xc,calibraX,calibraY;
int grados, pasos, calibraGrados, espera;
// Construir Stepper
CustomStepper stepper(8, 9, 10, 11);
int rotatedeg = 1;

void setup() {
  //iniciar comunicacion serial
  Serial.begin(9600);
  //configurar pines de sensores
  //ejes Y
  pinMode(trisen1, OUTPUT);
  pinMode(echosen1, INPUT);
  // ejes X
  pinMode(trisen2, OUTPUT);
  pinMode(echosen2, INPUT);
  // laser
  pinMode(laser, OUTPUT);
  // Agregar distancia entre eje y sensor 
  calibraX = 1;
  calibraY = 1;
  calibraGrados = -3;
  // Agregar distancia de RADIO
  radio = 5.8;
  // agrega segundos de espera
  espera =2;
  // Configurar motor velocidad y pasos por vuelta
  stepper.setRPM(6);
  //stepper.setSPR(4075.7728395);
  stepper.setSPR(4096);
  // llamar a la funcion para calcular distancias y grados a girar
  calculo();
}

void loop() {
  // comenzar a mover llamando la funcion mover
  if (stepper.isDone() && rotatedeg == 1){
  stepper.setDirection(CCW);
    stepper.rotateDegrees(grados+calibraGrados);
    rotatedeg = 2;
  }
    stepper.run();
  if (stepper.isDone() && rotatedeg == 2){
  Serial.println("Apuntando");
  digitalWrite(laser,HIGH);
  delay(espera*1000);
  digitalWrite(laser,LOW);
  rotatedeg = 3;  
  }
  if (stepper.isDone() && rotatedeg == 3){
    stepper.setDirection(CW);
    stepper.rotateDegrees(grados+calibraGrados);
    rotatedeg = 4;
  }
  stepper.run();
  if (stepper.isDone() && rotatedeg == 4){
    delay(5000);
    calculo();
    rotatedeg = 1;
  }
}
// declarar la funcion de calculo
double calculo(){
  //Calibrando sensores y tomando distancias

  //eje Y
  digitalWrite(trisen1, LOW);
  delayMicroseconds(2);
  digitalWrite(trisen1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trisen1, LOW);
  double duration1 = pulseIn(echosen1, HIGH);
  //eje X
  digitalWrite(trisen2, LOW);
  delayMicroseconds(2);
  digitalWrite(trisen2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trisen2, LOW);
  double duration2 = pulseIn(echosen2, HIGH);
  
//  // Convirtiendo a centimetros y guardar en variables de formula
  Yb = duration1  / 29 / 2;
  Serial.println("distancia en Y");
  Serial.println(Yb);
  Xc = duration2  / 29 / 2;
  Serial.println("distancia en X");
  Serial.println(Xc);
  
  //Calculando Grados usando Formula 
  // agregar valores de calibracion y valores estaticos a variables
  R = radio;
  Yb = Yb + calibraY;
  Xc = Xc + calibraX;
  // se divide la formula en 3 para mejor entendimiento
  double a = -2*(R*Yb);
  double b = sqrt((4*(pow(R,2)*pow(Yb,2)))+((4*(pow(Yb,2)+pow(Xc,2)))*(pow(Xc,2)-pow(R,2))));
  double c = 2*(pow(Yb,2)+pow(Xc,2));
  // se unen las 3 operaciones
  double resultado = (a+b)/c;
  //aplicar Cos-1 al resultado y convertirlo en grados
  resultado = degrees(acos(resultado));
  resultado = resultado+0.5;
  //convertir grados en un entero
  grados = (int)resultado;
  Serial.println("grados");
  Serial.println(grados);
}
