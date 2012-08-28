#define CAPACITANCE (1e-6)
#define RESISTOR1 (5e3)
#define BETA (3988.0)
#define RINF (10e3*exp(-BETA/298.15))
#define PERIODS 10

long int t=0;
long int t1;
void setup(){
delay(1000);
Serial.begin(9600);
pinMode(2,INPUT);}
void loop(){
  float thermistorResistance;
  float temp;
  float period;
  int i;
  while(digitalRead(2)==HIGH){};
    while(digitalRead(2)==LOW){};
    t=micros();
  for (i=0;i<PERIODS;i++) {
    while(digitalRead(2)==HIGH){};
    while(digitalRead(2)==LOW){};
  }
  t=micros()-t;
  period=t/PERIODS;
  thermistorResistance=(period*1e-6)/(2.0*log(2.0)*CAPACITANCE)-RESISTOR1/2.0;
  temp=BETA/log(thermistorResistance/RINF)-273.15;

  Serial.println(temp);
  delay(500);
}
