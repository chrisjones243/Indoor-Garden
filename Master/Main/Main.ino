// This will contain the main code for the master device
// and will be used to process the inputs and send the data to the slave device.

//Access the Wire library
#include <Wire.h>
//Set the button to a pin
int button = 6;
//Variable to check if the button is HIGH or LOW
int buttonState = 0;


void setup() {
  //Sets button to input
  pinMode(button, INPUT);
  //Starts I2C communication
  Wire.begin();
}

void loop() {
  buttonState = digitalRead(button);
  if (buttonState == HIGH){
    //Transmits the integer 1 in bytes to the slave arduino
    Wire.beginTransmission(8);
    Wire.write(1);
    Wire.endTransmission();
  }
  else{
    //Transmits the integer 0 in bytes to the slave arduino
    Wire.beginTransmission(8);
    Wire.write(0);
    Wire.endTransmission();
  }
}