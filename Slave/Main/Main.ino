
#include <Wire.h> //Access the Wire library
#include <LiquidCrystal.h> //Access the LiquidCrystal library

// LCD pins
int rs = 5, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;

// LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lightPin = 13;
int pumpPin = 12;
int lightLCD = 101;
int moistureLCD = 101;
int LEDLCD = 0; // This is not a constant
int pumpLCD = 0;

byte sun[] = {
  B10001,
  B11011,
  B01110,
  B11111,
  B11111,
  B01110,
  B11011,
  B10001
};

byte water[] = {
  B00011,
  B00111,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110
};
const char* moistureValue;

void setup() {
  //Sets the address of the arduino to 8
  Wire.begin(8);
  //Registers the event when the arduino recieves a transmission
  Wire.onReceive(receiveEvent);
  lcd.createChar(0, sun);
  lcd.createChar(1, water);
  pinMode(lightPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("LED:OFF");
  lcd.setCursor(8, 0);
  lcd.print("PUMP:OFF");
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.print(":");
  lcd.setCursor(8, 1);
  lcd.write(byte(1));
  lcd.print(":");
  //Starts the serial to output on the serial monitor
  //This is done to observe what signal the slave is receiving
  Serial.begin(9600);
}

void loop() {
}

void setLightState(bool state) {
  if (state == true){
    if (LEDLCD != 1){
      displayString("ON", 6, 0);
      LEDLCD = 1;
    }
  }
  else{
    if (LEDLCD != 0){
      displayString("OFF", 6, 0);
      LEDLCD = 0;
    }
  }
}

void setPumpState(bool state) {
  if (state == true){
    if (pumpLCD != 1){
      displayString("ON", 15, 0);
      pumpLCD = 1;
    }
  }
  else{
    if (pumpLCD != 0){
      displayString("OFF", 15, 0);
      pumpLCD = 0;
    }
  }
}

void displayValue(String value, int collumn, int row){
  const char* strValue = value.c_str();
  displayString(strValue, collumn, row);
}

void displayString(const char string[], int collumn, int row){
  lcd.setCursor(collumn - 2, row);
  lcd.print("   ");
  lcd.setCursor(collumn, row);
  lcd.rightToLeft();
  for (int i = strlen(string) - 1; i >= 0; i --){
    lcd.print(string[i]);
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
    if (lightLCD != command.substring(2).toInt()){
      displayValue(command.substring(2), 5, 1);
      lightLCD = command.substring(2).toInt();
    }   
  }
  //Checks if the command is a moisture value command
  if (command.substring(0, 2) == "MV") {
    if (moistureLCD != command.substring(2).toInt()){
      displayValue(command.substring(2), 12, 1);
      moistureLCD = command.substring(2).toInt();
    }
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