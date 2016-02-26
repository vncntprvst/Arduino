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
  delay(1000);              // wait for a second
  digitalWrite(LEDPin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
}
