/*
  LED
  Turns on an LED on for a set time, then off, repeatedly.
 */


// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
//  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
//  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(20);              // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  delay(80);  
}
