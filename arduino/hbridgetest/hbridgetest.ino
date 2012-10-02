#define PINQ 3
#define PINQBAR 5
int state;
void setup() {

pinMode(PINQ, OUTPUT);
pinMode(PINQBAR, OUTPUT);
pinMode(13,OUTPUT);
state=HIGH;
digitalWrite(13,state);
digitalWrite(PINQ,state);
digitalWrite(PINQBAR,!state);
}

void loop() {
  delay(20000);
  digitalWrite(PINQ,state);
  digitalWrite(PINQBAR,!state);
  digitalWrite(13,state);
  state=!state;
}
