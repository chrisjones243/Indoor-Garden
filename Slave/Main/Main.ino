
#include <Wire.h> //Access the Wire library
#include <LiquidCrystal.h> //Access the LiquidCrystal library

// LCD pins
int rs = 5, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;

// LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Set the LED and the pump to the pins
int LEDPin = 13;
int pumpPin = 12;

// Strores the current values displayed on the LCD 
// This is to minimise the refreshes of the LCD display to prevent flickering
char lightLCD[] = "101";
char moistureLCD[] = "101";
int LEDLCD = 0; // This is not a constant
int pumpLCD = 0;

// Custom character icons to represent light level and moisture level respectively 
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

void setup() {
  // Sets the address of the arduino to 8
  Wire.begin(8);
  // Registers the event when the arduino recieves a transmission
  Wire.onReceive(receiveEvent);
  // Assigns the characters to a number
  lcd.createChar(0, sun);
  lcd.createChar(1, water);
  // Sets the LED pin to output 
  pinMode(LEDPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  initialise();
  //Starts the serial to output on the serial monitor
  //This is done to observe what signal the slave is receiving
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

// Initialises the display on the the LCD
void initialise(){
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
}

// Makes sure the messages recieved is processed one at a time
// This function is called when the arduino recieves a transmission from the master
void receiveEvent(int bytes) {
  String data = "";
  while(Wire.available() > 0) {
    char c = Wire.read();
    data.concat(c);
  }
  //Serial.println(data); // Prints the data to the serial monitor for debugging
  processCommand(data);
}

// Decodes the message from the master Arduino to find out what process it needs to complete
void processCommand(String command) {
  String mode = command.substring(0, 2);
  String value = command.substring(2);
  // Checks if the command to control the LEDs
  if (mode == "LS") {
      setState(value, lightLCD, LEDPin, 6);
  }
  // Checks if the command is to turn the pump on or off
  else if (mode == "PS") {
     setState(value, pumpLCD, pumpPin, 15);
  }
  // Checks if the command is to display the light level
  else if (mode == "LV") {
    displayValue(value, 5, 1, lightLCD); 
  }
  // Checks if the command is to display the moisture level
  else if (mode == "MV") {
    displayValue(value, 12, 1, moistureLCD);
  }
}

// Sets the output component to HIGH or LOW based on the value of the nextState parameter
// Also prints out the status of the component on the LCD using displayString
void setState(String nextState, int outputLCD, int outputPin, int collumn){
  if (nextState == "1"){
    if (outputLCD != 1){     
      outputLCD = 1;
    }
    displayString("ON", collumn, 0);
    digitalWrite(outputPin, HIGH);
  }
  else{
    outputLCD = 0;
    displayString("OFF", collumn, 0);
    digitalWrite(outputPin, LOW);
  }
}

// Turns the String into a constant char so it can be displayed on the LCD
void displayValue(String value, int collumn, int row, char outputLCD[]){
  const char* strValue = value.c_str();
  if (outputLCD != strValue){
    displayString(strValue, collumn, row);
    outputLCD = strValue;
  }
  
}

// Clears the current display on the LCD and prints the new string onto the LCD
void displayString(const char string[], int collumn, int row){
  lcd.setCursor(collumn - 2, row);
  lcd.print("   ");
  lcd.setCursor(collumn, row);
  // Prints the string from right to left (e.g " 56" instead of "56 ")
  lcd.rightToLeft();
  // Prints the last value first from right to left
  for (int i = strlen(string) - 1; i >= 0; i --){
    lcd.print(string[i]);
  }
  lcd.leftToRight();
}
