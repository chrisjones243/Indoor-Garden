
#include <Wire.h> //Access the Wire library
#include <LiquidCrystal.h> //Access the LiquidCrystal library

// LCD pins
int rs = 5, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;

// LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int lightPin = 13;
int pumpPin = 12;
int lightVal = 0;
int moistureVal = 0;

void setup() {
  //Sets the address of the arduino to 8
  Wire.begin(8);
  //Registers the event when the arduino recieves a transmission
  Wire.onReceive(receiveEvent);
  pinMode(lightPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("LED:OFF");
  lcd.setCursor(8, 0);
  lcd.print("PUMP:OFF");
  lcd.setCursor(0, 1);
  lcd.print("LGT:");
  lcd.setCursor(8, 1);
  lcd.print("MSTR:");
  //Starts the serial to output on the serial monitor
  //This is done to observe what signal the slave is receiving
  Serial.begin(9600);
}

void loop() {
}

void setLightState(bool state) {
  if (state == true){
    lcd.setCursor(4, 0);
    lcd.print("   ");
    lcd.setCursor(6, 0);
    lcd.rightToLeft();
    lcd.print("NO");
  }
  else{
    lcd.setCursor(4, 0);
  	lcd.print("   ");
    lcd.setCursor(6, 0);
    lcd.rightToLeft();
    lcd.print("FFO");
  }
  lcd.leftToRight();
}

void setPumpState(bool state) {
  Serial.println(state);
  if (state == true){
    lcd.setCursor(13, 0);
  	lcd.print("   ");
    lcd.setCursor(15, 0);
    lcd.rightToLeft();
    lcd.print("NO");
  }
  else{
    lcd.setCursor(13, 0);
  	lcd.print("   ");
    lcd.setCursor(15, 0);
    lcd.rightToLeft();
    lcd.print("FFO");
  }
  lcd.leftToRight();
}

void displayLightVal(String val) {
  const char* lightValue = val.c_str();
  lcd.setCursor(4, 1);
  lcd.print("   ");
  lcd.setCursor(6, 1);
  lcd.rightToLeft();
  for (int i = strlen(lightValue) - 1; i >= 0; i --){
    lcd.print(lightValue[i]);
  }
  lcd.leftToRight();
}

void displayMoistureVal(String val) {
  const char* moistureValue = val.c_str();
  lcd.setCursor(13, 1);
  lcd.print("   ");
  lcd.setCursor(15, 1);
  lcd.rightToLeft();
  for (int i = strlen(moistureValue) - 1; i >= 0; i --){
    lcd.print(moistureValue[i]);
  }
  lcd.leftToRight();
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
    displayLightVal(command.substring(2));
  }
  //Checks if the command is a moisture value command
  if (command.substring(0, 2) == "MV") {
    //Sets the moisture value to the value in the command
    displayMoistureVal(command.substring(2));
  }
}

void receiveEvent(int bytes) {
  //This function is called when the arduino recieves a transmission
  String data = "";
  while(Wire.available() > 0) {
    char c = Wire.read(); //Reads the data from the master
    data.concat(c); //Adds the data to the string
  }
  //Serial.println(data); //Prints the data to the serial monitor
  processCommand(data); //Processes the command
}