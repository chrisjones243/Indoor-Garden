// This will contain the main code for the slave device
// and will be used to process the outputs using data from the master device.
#include <Wire.h>

int LED = 9;

void setup() {
  //Sets the address of the arduino to 8
  Wire.begin(8);
  //Registers the event when the arduino recieves a transmission
  Wire.onReceive(receiveEvent);
  pinMode(LED, OUTPUT);
  //Starts the serial to output on the serial monitor
  //This is done to observe what signal the slave is receiving
  Serial.begin(9600);
}

void loop() {
}

void receiveEvent(int howMany){
  int state = Wire.read();
  //prints out the signal recieved
  Serial.print(state);
  //If the signal is integer 1, the LED is switched on
  if (state == 1){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
}