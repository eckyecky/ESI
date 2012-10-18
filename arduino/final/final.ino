#include <stdio.h>

//PID defines
#define DT 100000 //time in micros
#define KP_CONST  (255.0/7.0)
#define KI_CONST  (255.0/10.0)
#define KD_CONST  0

// defining various sensor circut constants
#define CAPACITANCE (1e-6)
#define RESISTOR1 (5e3)
#define BETA (3988.0)
#define RINF (10e3*exp(-BETA/298.15))
#define CELCIUS_OFFSET 273.15

//INPUT PINs 2, 3, 4, 5, 6, 7
#define INPUTS 6
#define INSTART 2
#define TEMPPIN  1

//OUTPUT PINS
#define OUTPUTS  4
#define SIGNPIN  8

// Pulse Width PINs for controlling Cooler
#define PINQ 11
#define PINQBAR 10

#define DEBUG_TEMP 37


//OUTPUT array
const int outputs[]={8,9,12,13};


void setup() {
  delay(1000);
  Serial.begin(9600);
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
  const float Kp=KP_CONST,Ki=KI_CONST,Kd=KD_CONST;
  const long dt=DT;
  float derivative;
  float error;
  float output;
  while (micros()-time<dt);
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
  Serial.print("measured = ");
  Serial.println(temp,2); //Debug print temp
  temp+= temp*0.2006-6.8166;      //Statistical Error fix
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
  digitalWrite(outputs[1],(value&(1<<2)?HIGH:LOW));
  digitalWrite(outputs[2],(value&(1<<1)?HIGH:LOW));
  digitalWrite(outputs[3],(value&(1<<0)?HIGH:LOW));  
}
void loop(){
  int level;
  int inputTemp=readInput();
  //int inputTemp=DEBUG_TEMP;
  float temp=handleTemp();
  //Serial.println(inputTemp);
  displayOutput(inputTemp-round(temp));
  level=(int)PIDcontrol(inputTemp,temp);
  Serial.print("PID = ");
  Serial.println(level);
  Serial.print("Real = ");
  Serial.println(temp);
  if (level<0) {
    analogWrite(PINQBAR,-1*level);
    analogWrite(PINQ,0);
  } else {
    analogWrite(PINQ,level);
    analogWrite(PINQBAR,0);
  }
}
