long int t=0;
long int t1;
void setup(){
delay(1000);
Serial.begin(9600);
pinMode(2,INPUT);}
void loop(){
  while(digitalRead(2)==HIGH){};
  while(digitalRead(2)==LOW){};
  t= micros();
  while(digitalRead(2)==HIGH){};
  while(digitalRead(2)==LOW){};
  t=micros()-t;
  t1=millis();
  //Serial.print(t1);Serial.print(" ");
  Serial.println(t);
  delay(500);
}
