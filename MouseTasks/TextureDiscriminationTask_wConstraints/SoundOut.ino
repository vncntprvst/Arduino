void SoundOut(int instruct){
//HIGH triggers trinkets listening
//Serial.println("TTL out");
  switch (instruct) {
    // both cases should total 10ms
    case 1: 
    // white noise
      digitalWrite(SoundTriggerPin, HIGH); // trigger
// NB: 
// if control PlayTone enabled in WhiteNoise_USbeep
// need to add a total of 10ms (or control beep's duration)
// to wait for if statment 
      delay(2); 
      digitalWrite(SoundTriggerPin, LOW); // White noise
      delay(8);
      break;
    case 2:
      // US
      digitalWrite(SoundTriggerPin, HIGH); // trigger
      delay(2); 
      digitalWrite(SoundTriggerPin, LOW);
      delay(2);
      digitalWrite(SoundTriggerPin, HIGH); // +1
      delay(2);
      digitalWrite(SoundTriggerPin, LOW);
      delay(4);
    break;
   }
}
