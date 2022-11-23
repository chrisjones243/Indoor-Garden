// Code from the WCRS Website https://wcrsyyc.github.io/ardx/circ09.html
int lightPin = 0; 
int ledPin = 9;
void setup()
{
  pinMode(ledPin, OUTPUT); //sets the led pin to output
}

void loop()
{
  int lightLevel = analogRead(lightPin);
  lightLevel = map(lightLevel, 0, 1000, 0, 100);
  lightLevel = constrain(lightLevel, 0, 100);
}