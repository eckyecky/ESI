#include <stdio.h>

//PID defines
#define DT 100000 //time in micros

// defining various sensor circut constants
#define CAPACITANCE (1e-6)
#define RESISTOR1 (5e3)
#define BETA (3988.0)
#define RINF (10e3*exp(-BETA/298.15))
#define CELCIUS_OFFSET 273.15

//INPUT PINs 7,8,9,10,11,12
#define INPUTS 6
#define INSTART 7
#define TEMPPIN  0

//OUTPUT PINS
#define OUTPUTS  4
#define SIGNPIN  13
#define OUT_0    6
#define OUT_1    4
#define OUT_2    2

// Pulse Width PINs for controlling Cooler
#define PINQ 3
#define PINQBAR 5

//OUTPUT array
const int outputs[]={2,4,6,13};


void setup() {
  delay(1000);
  //Set output pins mode
  for (int i=0;i<OUTPUTS;i++) {
    pinMode(outputs[i],OUTPUT);
  }
  //Set input pins mode
  pinMode(TEMPPIN,INPUT);
  for (int i=INSTART;i<INPUTS+INSTART;i++) {
    pinMode(i,INPUT);
    digitalWrite(i,HIGH); //use internal pull-ups
  }
  analogWrite(PINQ,0);
  analogWrite(PINQBAR,0);
}


float PIDcontrol(int settemp,float temp) {
  static long time=micros();
  static float previous_error=settemp-temp;
  static float integral=0;
  const float Kp=0,Ki=0,Kd=0;
  const int dt=DT;
  float derivative;
  float error;
  float output;
  while (micros()-time<dt) ;
  time=micros();
  error=settemp-temp;
  integral = integral+(error*dt);
  derivative = (error - previous_error)/dt;
  output = (Kp*error) + (Ki*integral) + (Kd*derivative);
  previous_error = error;
  return output>255?255:(output<-255?-255:output);
}


// Reads and prints temperature to serial
float handleTemp() {
  long int t=0 ,t1;
  float Rt; // thermistor resistance
  float temp;
  float period;
  int i;

  // read period
  while(digitalRead(TEMPPIN)==HIGH) {}; 
  while(digitalRead(TEMPPIN)==LOW ) {};
  t = micros();
  while(digitalRead(TEMPPIN)==HIGH) {};
  while(digitalRead(TEMPPIN)==LOW ) {};
  t = micros()-t;
  period = t*1e-6; //converts to average and from (microsec) to (sec)
  Rt = (period)/(2.0*log(2.0)*CAPACITANCE)-RESISTOR1/2.0;

  temp = BETA/log(Rt/RINF) - CELCIUS_OFFSET ;

//  Serial.println(temp,2); //Debug print temp
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
  for (int i=0;i<OUTPUTS;i++) {
    digitalWrite(outputs[i],(value&(1<<(i-3))?HIGH:LOW));
  }
    
}
void loop(){
  int level;
  int inputTemp=readInput();
  float temp=handleTemp();
  displayOutput(inputTemp-round(temp));
  level=(int)PIDcontrol(inputTemp,temp);
  if (level<0) {
    analogWrite(PINQBAR,-1*level);
    analogWrite(PINQ,0);
  } else {
    analogWrite(PINQ,level);
    analogWrite(PINQBAR,0);
  }
}
