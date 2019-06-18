
const int  TTLPin = 11;    // the pin that the pushbutton is attached to
const int ledPin = 13;       // the pin that the LED is attached to
int TTLstate = 0;         // current state of the button

void setup() {
  pinMode(TTLPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
//  TTLstate = digitalRead(TTLPin);
//    if (TTLstate == HIGH) {
//      Serial.println(5);
//      digitalWrite(ledPin, HIGH);
//    } else {
//      Serial.println(0);
//      digitalWrite(ledPin, LOW);
//    }
digitalWrite(ledPin, HIGH);
delay(20);
digitalWrite(ledPin, LOW);
delay(180);
}
