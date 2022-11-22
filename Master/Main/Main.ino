// This will contain the main code for the master device
// and will be used to process the inputs and send the data to the slave device.

//Access the Wire library
#include <Wire.h>

int button = 6; //Set the button to a pin
int buttonState = 0; //Variable to check if the button is HIGH or LOW

// NOTE: When transmitting data, to the slave device, the master device will need to send these values:
// LS[boolean] = lightState, the state of the light
// PS[boolean] = pumpState, the state of the pump
// LV[int] = lightval, the value of the light
// MV[int] = moistureval, the value of the moisture
// 
// e.g.
// LS1 = lightState is true, and will turn on the light on
// PS0 = pumpState is false, and will turn off the pump
// LV100 = lightval is 100, and will display the value 100 on the LCD
// 
// **Can code on slave-output branch**


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