#include <CustomStepper.h>
#include <math.h>
//definir motor pasos por revolucion y pines
#define trisen1 2
#define echosen1 3
#define trisen2 4
#define echosen2 5
char tecla;
double R, Yb,Xc,calibraX,calibraY;
int grados;
int pasos;
CustomStepper stepper(8, 9, 10, 11);
int rotatedeg = 1;
void setup() {
  Serial.begin(9600);
  pinMode(trisen1, OUTPUT);
  pinMode(echosen1, INPUT);
  pinMode(trisen2, OUTPUT);
  pinMode(echosen2, INPUT);
  calibraX = 0;
  calibraY = 0;
  stepper.setRPM(12);
  stepper.setSPR(4075.7728395);
  calculo();
}

void loop() {

  mover();
}
void mover(){

  if (stepper.isDone() && rotatedeg == 1)
  {
    stepper.setDirection(CCW);
    stepper.rotateDegrees(grados);
    rotatedeg = 2;
  }
  stepper.run();
    if (stepper.isDone() && rotatedeg == 2)
  {
    stepper.setDirection(CW);
    stepper.rotateDegrees(grados);
    rotatedeg = 3;
  }
  stepper.run();
}

double calculo(){
  //Calibrando sensores
  digitalWrite(trisen1, LOW);
  delayMicroseconds(2);
  digitalWrite(trisen1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trisen1, LOW);
  double duration1 = pulseIn(echosen1, HIGH);

  digitalWrite(trisen2, LOW);
  delayMicroseconds(2);
  digitalWrite(trisen2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trisen2, LOW);
  double duration2 = pulseIn(echosen2, HIGH);
  
//  // Convirtiendo a centimetros
  double cm1 = duration1  / 29 / 2;
  Serial.println("distancia en Y");
  Serial.println(cm1);
  double cm2 = duration2  / 29 / 2;
  Serial.println("distancia en X");
  Serial.println(cm2);
  //Calculando Grados usando Formula
  R = 2;
  Yb = cm1 + calibraY;
  Xc = cm2 + calibraX;
  double a = -2*(R*Yb);
  //Serial.println("a");
  //Serial.println(a);
  double b = sqrt((4*(pow(R,2)*pow(Yb,2)))+((4*(pow(Yb,2)+pow(Xc,2)))*(pow(Xc,2)-pow(R,2))));
  //Serial.println("b");
  //Serial.println(b);
  double c = 2*(pow(Yb,2)+pow(Xc,2));
  //Serial.println("c");
  //Serial.println(c);
  double resultado = (a+b)/c;
  //Serial.println("resultado 1");
  //Serial.println(resultado);
  resultado = degrees(acos(resultado));
  //Serial.println("resultado 2");
  //Serial.println(resultado);
  resultado = resultado+0.5;
  grados = (int)resultado;
  Serial.println("grados");
  Serial.println(grados);
}
