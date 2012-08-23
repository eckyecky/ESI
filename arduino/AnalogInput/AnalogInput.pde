
void setup() {
  pinMode(13, OUTPUT);    
 delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  
  
  digitalWrite(13, HIGH);
  

  Serial.begin(9600);
  
  for(int i =0;i<1000;i++){
    int sensorValue = analogRead(A0);
    if(((i^0xc) %50 ) == 0)
        digitalWrite(13, HIGH);
    if(((
    i^0xc) %50 ) == 25)
        digitalWrite(13, LOW);
    Serial.println(sensorValue);
  }
}

void loop() {
  delay(50);
  /*int sensorValue = analogRead(A0)
  ;
  
  
  
  
  digitalWrite(13, LOW);
   Serial.println(sensorValue);
  */
}

