unsigned long tempsactuel;
//unsigned long resultat;

void setup() {
    Serial.begin(9600);
}

void loop() {
  tempsactuel = millis();
  Serial.print(millis() + 1000);
  Serial.print(" ");
  Serial.println(tempsactuel + 1000);
  delay(500);
}
