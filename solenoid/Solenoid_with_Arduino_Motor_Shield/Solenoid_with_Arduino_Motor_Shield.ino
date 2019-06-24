//Run Solenoid with Arduino Motor Shield

void setup() {
  //Motor A
  pinMode(12, OUTPUT); //Motor A Direction pin
  pinMode(9, OUTPUT); //Motor A Brake pin
  digitalWrite(12, HIGH); //Set forward direction
}

void loop() {
  // open 
  digitalWrite(9, LOW);  // Disengage the brake
  analogWrite(3, 255);   // Run at max value
  delay(3000);           // Keep open
  // close
  digitalWrite(9, HIGH); // Engage the brake 
  delay(1000);           // Keep close
}
