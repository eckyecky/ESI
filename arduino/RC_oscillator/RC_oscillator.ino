int readPIN = 2;
int startTime=0;
int curTime=0;
int period=0;
void setup() {
 Serial.begin(9600);
 startTime=micros();
 attachInterrupt(0,risingCall,RISING);
}


void loop() {
  
}

void risingCall () {
  curTime=micros();
  period=curTime-startTime;
  Serial.println(period);
  startTime=curTime;
}
