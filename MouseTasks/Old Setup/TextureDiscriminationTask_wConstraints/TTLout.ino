void TTLout(int instruct){
  digitalWrite(TTLPin, HIGH);
  delay(10);
  digitalWrite(TTLPin, LOW);
  switch (instruct) {
    case 1: 
    // trial initiation: 2 TTL
    delay(10);
    digitalWrite(TTLPin, HIGH);
    delay(10);
    digitalWrite(TTLPin, LOW);
    break;
    case 2:
  // that's it
    break;
  }
}
