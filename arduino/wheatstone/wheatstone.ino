/*

  Smoothing

  Reads repeatedly from an analog input, calculating a running average
  and printing it to the computer.  Keeps ten readings in an array and 
  continually averages them.
  
  The circuit:
    * Analog sensor (potentiometer will do) attached to analog input 0

  Created 22 April 2007
  By David A. Mellis  <dam@mellis.org>
  modified 9 Apr 2012
  by Tom Igoe
  http://www.arduino.cc/en/Tutorial/Smoothing
  
  This example code is in the public domain.


*/


// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.

int thermistorPin = A1;
int otherPin = A0;

int thermRead=0;
int otherRead=0;

double vout=0;
double resistance=0;
void setup()
{
  // initialize serial communication with computer:
  Serial.begin(9600);                 
 // initialize all the readings to 0: 
          
}

void loop() {
  otherRead=analogRead(otherPin);
  thermRead=analogRead(thermistorPin);
  Serial.println(otherRead);   
  delay(100);        // delay in between reads for stability            
}


