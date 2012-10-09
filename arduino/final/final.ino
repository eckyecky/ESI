#include <stdio.h>
// defining various sensor circut constants
#define CAPACITANCE (1e-6)
#define RESISTOR1 (5e3)
#define BETA (3988.0)
#define RINF (10e3*exp(-BETA/298.15))
#define CELCIUS_OFFSET 0
//273.15
#define INPUTS 6
#define INSTART 7
#define SIGNPIN 6
#define TEMPPIN 13
#define 

#define PERIODS 1 // number of clock periods to average 

#define PINQ 3
#define PINQBAR 5
int state;
void setup() {
delay(1000);
  Serial.begin(9600);
  pinMode(TEMPPIN,INPUT);
  for (int i=INSTART;i<INPUTS+INSTART;i++) {
    pinMode(i,INPUT);
    digitalWrite(i,HIGH); //use internal pull-ups
  }
  for (int i=3;i<7;i++ ) {
    pinMode(i,OUTPUT); 
  }
pinMode(PINQ, OUTPUT);
pinMode(PINQBAR, OUTPUT);
state=HIGH;
digitalWrite(PINQ,state);
digitalWrite(PINQBAR,!state);
}


void PIDcontrol(int settemp,float temp) {
  static long time=micros(); //test only initial conditionage
  static float previous_error=settemp-temp; //test only initial conditionage
  static float integral=0; //testr this only sets on initial initialisation
  const float Kp,Ki,Kd;
  const int dt;
  while (micros()-time<dt) ;
  time=micros;
  error=setpoint-temp;
  integral = integral+(error*dt);
  derivative = (error - previous_error)/dt
  output = (Kp*error) + (Ki*integral) + (Kd*derivative)
  previous_error = error
}


// Reads and prints temperature to serial
float handleTemp() {
  long int t=0 ,t1;
  float Rt; // thermistor resistance
  float temp;
  float period;
  int i;

  // read average period
  while(digitalRead(TEMPPIN)==HIGH) {}; 
  while(digitalRead(TEMPPIN)==LOW ) {};
  t = micros();

  for (i=0;i<PERIODS;i++) {
    while(digitalRead(TEMPPIN)==HIGH) {};
    while(digitalRead(TEMPPIN)==LOW ) {};
  }

  t = micros()-t;
  period = t/PERIODS*1e-6; //converts to average and from (microsec) to (sec)
  Rt = (period)/(2.0*log(2.0)*CAPACITANCE)-RESISTOR1/2.0;
  //Serial.println(Rt);
  temp = BETA/log(Rt/RINF) - CELCIUS_OFFSET ;

  Serial.println(temp,2);
  return temp;
}

int readInput() {
  int setValue=0;
  for (int i=INSTART;i<INPUTS+INSTART;i++) {
    setValue+=(digitalRead(i)==HIGH?0:1)<<(i-INSTART);
  }
  return setValue;
}  

void displayOutput(int value) {
  digitalWrite(SIGNPIN,value<0?HIGH:LOW);
  value=abs(value);
  if (value>7) value=7;
  for (int i=3;i<6;i++) {
    digitalWrite(i,(value&(1<<(i-3))?HIGH:LOW));
  }
    
}
void loop(){
  Serial.println("Temp");
  //int inputTemp=  readInput();
  int temp= handleTemp();
  //displayOutput(inputTemp-round(temp));
  //Serial.println(inputTemp-temp);
  delay(2000); // used to limit serial output
}
