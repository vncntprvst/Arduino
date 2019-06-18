/*
  LED
  Turns on an LED on for a set time, then off, repeatedly.
 */


// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(9, OUTPUT);
//  pinMode(8, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
//  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);              // wait for a second
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
//  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  delay(90);  
}
