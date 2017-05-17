/*
  LED
  Turns on an LED on for a set time, then off, repeatedly.
 */
const int LEDPin = 22;
const int TTLPin = 46;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LEDPin, OUTPUT);
  pinMode(TTLPin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(TTLPin, HIGH);
  delay(25);              // wait 
  digitalWrite(LEDPin, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(TTLPin, LOW);
  delay(100);              // wait 
  digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(TTLPin, HIGH);
  delay(25);              // wait for a secon
  digitalWrite(LEDPin, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(TTLPin, LOW);
  delay(500);  
}
