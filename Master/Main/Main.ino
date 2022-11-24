// This will contain the main code for the master device
// and will be used to process the inputs and send the data to the slave device.

//Access the Wire library
#include <Wire.h>

int lightPin = 0; // Sets the photoresistor to a pin
int time = 0; // Keeps track of how many seconds has passed

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
  Wire.begin(); // Starts I2C communication
  Serial.begin(9600); // Serial communication
}

// Each loop in the program is called every second
void loop() {
  Serial.println(time); // Prints the time to the serial monitor so we know when the functions will do their tasks
  transmit("LV", readLightLevel()); // Transmits the light level to the slave
  checkLightLevel();
  delay(1000);
  time ++; 
  }

//Transmits the command mode and the value to the slave
void transmit(char mode[], int value){
  Wire.beginTransmission(8);
      Wire.write(mode);
      Wire.write(value);
      Wire.endTransmission();
}

// Returns the light level detected by the arduino from 0 to 100
int readLightLevel(){
      int lightLevel = analogRead(lightPin);
      lightLevel = map(lightLevel, 0, 1000, 0, 100);
      lightLevel = constrain(lightLevel, 0, 100);
      return lightLevel;
}

// Communicates the slave to switch the light bulb on or off depending on the light level
void checkLightLevel(){
  // Only checks the light level every 10 seconds 
  if(time % 10 == 0){
    int threshold = 20;
    if(readLightLevel() <= 20){
      transmit("LS", 1);
    }
    else{
      transmit("LS", 0);
    }
  }
}
