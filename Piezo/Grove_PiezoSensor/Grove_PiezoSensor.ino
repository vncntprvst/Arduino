const int piezoPin = A0;
const int ledPin = 13;      // led connected to digital pin 13
const int threshold = 100;

int sensorValue = 0;      // variable to store the value read from the sensor pin
int ledState = LOW;  

void setup() {
    Serial.begin(9600);
    pinMode(ledPin,OUTPUT);
}

void loop() {
    sensorValue = analogRead(piezoPin);
    Serial.println(sensorValue);
  // if the sensor reading is greater than the threshold:
  if (sensorValue >= threshold) {
    // toggle the status of the ledPin:
    ledState = !ledState;
    // update the LED pin itself:
    digitalWrite(ledPin, ledState);
    // send the string "Knock!" back to the computer, followed by newline
    Serial.println("Vibration!");
  }
  delay(100);  // delay to avoid overloading the serial port buffer
}



