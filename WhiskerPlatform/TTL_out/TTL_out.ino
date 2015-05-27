
const int TTLPin = 12;
 
 void setup() {
   Serial.begin(9600); 
   // set output pin
   pinMode(TTLPin, OUTPUT);
 }
 
 void loop() {
Serial.println("UP");
digitalWrite(TTLPin, HIGH);
delay(500);
Serial.println("DOWN");
digitalWrite(TTLPin, LOW);
delay(100);
}
