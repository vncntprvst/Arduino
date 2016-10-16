void TTLout(int instruct){
  digitalWrite(TTLPin, HIGH);
  delay(25);
  digitalWrite(TTLPin, LOW);
  switch (instruct) {
    case 1: 
    // trial initiation: 2 TTL
    delay(25);
    digitalWrite(TTLPin, HIGH);
    delay(25);
    digitalWrite(TTLPin, LOW);
    break;
    case 2:
  // that's it
    break;
  }
}
