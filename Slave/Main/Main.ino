// This will contain the main code for the slave device
// and will be used to process the outputs using data from the master device.
#include <Wire.h>
#include <Arduino.h>


int lightPin = 13;
int pumpPin = 12;
int lightVal = 0;
int moistureVal = 0;

String data = "";

void setup() {
  //Sets the address of the arduino to 8
  Wire.begin(8);
  //Registers the event when the arduino recieves a transmission
  Wire.onReceive(receiveEvent);
  pinMode(lightPin, OUTPUT);
  //Starts the serial to output on the serial monitor
  //This is done to observe what signal the slave is receiving
  Serial.begin(9600);
}

void loop() {
}

void setLightState(bool state) {
  digitalWrite(lightPin, state);
}

void setPumpState(bool state) {
  digitalWrite(pumpPin, state);
}

void displayLightVal(int val) {
  lightVal = val;
}

void displayMoistureVal(int val) {
  moistureVal = val;
}

void receiveEvent(int bytes) {
  //This function is called when the arduino recieves a transmission
  while(Wire.available() > 0) {

    char c = Wire.read(); //Reads the data from the master
    data.concat(c); //Adds the data to the string

    Serial.println(data); //Prints the data to the serial monitor

    // LS = lightState, PS = pumpState, LV = lightval, MV = moistureval

    if(data == "LS") {
      int lightState = Wire.read(); //Reads the next byte from the master
      if(lightState == 1) {
        setLightState(true);
      } else {
        setLightState(false);
      }
      data = "";
    }

    if (data == "PS") {
      int pumpState = Wire.read(); //Reads the next byte from the master
      if(pumpState == 1) {
        setPumpState(true);
      } else {
        setPumpState(false);
      }
      data = "";
    }

    if (data == "LV") {
      int lightval = Wire.read(); //Reads the next byte from the master
      displayLightVal(lightval); //Displays the light value
      data = "";
    }
    if (data == "MV") {
      int moistureval = Wire.read(); //Reads the next byte from the master
      displayMoistureVal(moistureval); //Displays the moisture value
      data = "";
    }
  }
}