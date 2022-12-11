// This will contain the main code for the master device
// and will be used to process the inputs and send the data to the slave device.

//Access the Wire library
#include <Wire.h>

int buttonPin = 6;
int LDRPin = 0; // Sets the photoresistor to a pin
int moisturePin = 1; // Sets the moisture to a pin

int time = 0; // The unit is in seconds
int buttonState; 
int previousTime = 0; // Stores an instance of the time so it can be compared with the current time
int automaticPump = false; // A flag to check if checkMoistureLevel() is currently transmitting a signal to turn the pump on

// NOTE: When transmitting data, to the slave device, the master device will need to send these values in a character array:
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

void setup() {
  // Starts I2C communication
  Wire.begin();
  // Starts Serial communication
  Serial.begin(9600); 
}

// Each loop in the program is called every second
void loop() {
  // Transmits the light and moisture level to the slave
  transmit("MV", readValue(moisturePin)); 
  transmit("LV", readValue(LDRPin));
  checkMoistureLevel();
  checkLightLevel();
  //This is so the program checks if the button is pressed over the course of one second
  for (int x = 0; x < 10; x ++){
    detectButtonInput();
    delay(100);
  }
  time ++; 
  }

//Transmits the command mode and the value to the slave
void transmit(char mode[], int value){
  char message[6];
  char strValue[4];
  // Converts value in to a string and concatenates the mode and the value together
  sprintf(strValue, "%d", value);
  strcpy(message, mode);
  strcat(message, strValue);
  Wire.beginTransmission(8);
  Wire.write(message);
  Wire.endTransmission();
}


// Communicates the slave to switch the light bulb on or off depending on the light level
void checkLightLevel(){
  // Only checks the light level every 10 seconds 
  if (time % 3 == 0){
    if(readValue(LDRPin) <= 3){
      transmit("LS", 1);
    }
    else{
      transmit("LS", 0);
  }
}
}

// Communicates to the slave to open the water pump depending on the moisture in the soil
// If the pump needs to be open, automaticPump is set to true so detectButtonInput() does not affect the master communication
void checkMoistureLevel(){
  if (time % 8== 0){
    if (readValue(moisturePin) >= 60){
      previousTime = time;
      automaticPump = true;
      transmit("PS", 1);
    }
  }
  if (time == previousTime + 4){
      automaticPump = false;
      transmit("PS", 0);
  }
}

// Sends a signal to the slave to turn the pump on when the button is pushed down
void detectButtonInput(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
    transmit("PS", 1);
  }
  else if (automaticPump == false){
    transmit("PS", 0);
  }
}


// Reads the anologue values of the pins and maps them from 0 to 100
int readValue(int pin){
  int reading = analogRead(pin);
  reading = map(reading, 0, 1024, 0, 100);
  reading = constrain(reading, 0, 100);
  return reading;
}




