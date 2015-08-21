#include <Stepper.h>
#include <math.h>
//definir motor pasos por revolucion y pines
Stepper motor(100, 8, 10, 9, 11);
double R, Yb,Xc;
int grados;
int pasos;
const int sensor1 = 6;
const int sensor2 = 7;
void setup() {
  Serial.begin(9600);
}

void loop() {
  
  // ejecutando calculo 
  Serial.println(grados);
  Serial.println(pasos);
  calculo();
  mover();
  
}

void mover(){
  int total = (grados *4096)/360;
  int retorno =total*(-1);
  pasos = total;
  motor.setSpeed(100);
  motor.step(total);
  delay(1000);
  motor.step(retorno);
  
}


double calculo(){
  //Calibrando sensores
  pinMode(sensor1, OUTPUT);
  digitalWrite(sensor1, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor1, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensor1, LOW);
  pinMode(sensor2, OUTPUT);
  digitalWrite(sensor2, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor2, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensor2, LOW);

  // detectando distancias en milisegundos

  pinMode(sensor1, INPUT);
  double duration1 = pulseIn(sensor1, HIGH);
  pinMode(sensor2, INPUT);
  double duration2 = pulseIn(sensor2, HIGH);

  // Convirtiendo a centimetros
  double cm1 = duration1  / 29 / 2;
  double cm2 = duration2  / 29 / 2;
  
  //Calculando Grados usando Formula
  
  double a = -2*(R*Yb);
  double b = sqrt((4*(pow(R,2)*pow(Yb,2)))+((4*(pow(Yb,2)+pow(Xc,2)))*(pow(Xc,2)-pow(R,2))));
  double c = 2*(pow(Yb,2)+pow(Xc,2));
  double resultado = (a+b)/c;
  Serial.begin(9600);
  Serial.print(resultado);
  resultado = resultado+0.5;
  grados = (int)resultado;
}
