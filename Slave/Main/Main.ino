// This will contain the main code for the slave device
// and will be used to process the outputs using data from the master device.

#include <Wire.h> //Access the Wire library
#include <Arduino.h> //Access the Arduino library


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


void displayLightVal(int val) {
  lightVal = val;
}

void displayMoistureVal(int val) {
  moistureVal = val;
}

void processCommand(String command) {
  //Checks if the command is a light state command
  if (command.substring(0, 2) == "LS") {
    //Checks if the light state is on or off
    if (command.substring(2, 3) == "1") {
      setLightState(true);
    } else {
      setLightState(false);
    }
  }
  //Checks if the command is a pump state command
  if (command.substring(0, 2) == "PS") {
    //Checks if the pump state is on or off
    if (command.substring(2, 3) == "1") {
      setPumpState(true);
    } else {
      setPumpState(false);
    }
  }
  //Checks if the command is a light value command
  if (command.substring(0, 2) == "LV") {
    //Sets the light value to the value in the command
    displayLightVal(command.substring(2).toInt());
  }
  //Checks if the command is a moisture value command
  if (command.substring(0, 2) == "MV") {
    //Sets the moisture value to the value in the command
    displayMoistureVal(command.substring(2).toInt());
  }
}

void receiveEvent(int bytes) {
  //This function is called when the arduino recieves a transmission
  while(Wire.available() > 0) {
    char c = Wire.read(); //Reads the data from the master
    data.concat(c); //Adds the data to the string
    Serial.println(data); //Prints the data to the serial monitor
  }
  processCommand(data); //Processes the command
}