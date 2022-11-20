// This will contain the main code for the slave device
// and will be used to process the outputs using data from the master device.
#include <Wire.h>

int LED = 9;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void receiveEvent(int howMany){
  int state = Wire.read();
  Serial.print(state);
  if (state == 1){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
}