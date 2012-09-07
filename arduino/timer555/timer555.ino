// defining various sensor circut constants
#define CAPACITANCE (1e-6)
#define RESISTOR1 (5e3)
#define BETA (3988.0)
#define RINF (10e3*exp(-BETA/298.15))
#define CELCIUS_OFFSET 273.15

#define PERIODS 10 // number of clock periods to average 

void setup(){
  delay(1000);
  Serial.begin(9600);
  pinMode(2,INPUT);
}
void loop(){

  long int t=0 ,t1;
  float Rt; // thermistor resistance
  float temp;
  float period;
  int i;

  // read average period
  while(digitalRead(2)==HIGH) {}; 
  while(digitalRead(2)==LOW ) {};
  t = micros();

  for (i=0;i<PERIODS;i++) {
    while(digitalRead(2)==HIGH) {};
    while(digitalRead(2)==LOW ) {};
  }

  t = micros()-t;
  period = t/PERIODS*1e-6; //converts to average and from (microsec) to (sec)
  Rt = (period)/(2.0*log(2.0)*CAPACITANCE)-RESISTOR1/2.0;
  temp = BETA/log(Rt/RINF) - CELCIUS_OFFSET ;

  Serial.println(temp);
  delay(500); // used to limit serial output
}
