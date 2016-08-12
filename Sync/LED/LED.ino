/*
  LED
  Turns on an LED on for a set time, then off, repeatedly.
 */
const int LEDPin = 12;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LEDPin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);              // wait 
  digitalWrite(LEDPin, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(10);              // wait 
  digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);              // wait for a secon
  digitalWrite(LEDPin, LOW);    // turn the LED off by making the voltage LOW
  delay(500);  
}
