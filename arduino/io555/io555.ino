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

#define PERIODS 1 // number of clock periods to average 

void setup(){
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
  Serial.println(Rt);
  temp = BETA/log(Rt/RINF) - CELCIUS_OFFSET ;

  Serial.println(temp);
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
  int inputTemp=  readInput();
  int temp= handleTemp();
  displayOutput(inputTemp-round(temp));
  Serial.println(inputTemp-temp);
  delay(2000); // used to limit serial output
}
