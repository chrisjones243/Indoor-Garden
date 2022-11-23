// This will contain the main code for the master device
// and will be used to process the inputs and send the data to the slave device.

//Access the Wire library
#include <Wire.h>

int button = 6; // Set the button to a pin
int buttonState = 0; // Variable to check if the button is HIGH or LOW
int lightPin = 0; // Sets the photoresistor to a pin

int time = 0; //Time in seconds

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
  // Starts I2C communication
  Wire.begin();
}

void loop() {
  transmit("LV", readLightLevel());
  checkLightLevel();
  delay(1000);
  time ++; 
  }

int readLightLevel(){
      int lightLevel = analogRead(lightPin);
      lightLevel = map(lightLevel, 0, 1000, 0, 100);
      lightLevel = constrain(lightLevel, 0, 100);
      return lightLevel;
}


void checkLightLevel(){
  if(time % 10 == 0){
    int threshold = 20;
    if(readLightLevel() <= 20){
      transmit("LS", 1)
    }
    else{
      transmit("LS", 0);
    }
  }
}

void transmit(char mode[], int value){
  Wire.beginTransmission(8);
      Wire.write(mode);
      Wire.write(value);
      Wire.endTransmission();
}