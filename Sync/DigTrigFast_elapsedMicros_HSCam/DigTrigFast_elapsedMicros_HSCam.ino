// Trigger signal for High-speed camera
const int syncPin = 0;
const int syncTrigPin = 1;
elapsedMicros sinceOnsetTest;
elapsedMicros pulseDuration;
//int counter = 0;
bool syncOn = false; //true

void setup() {
  pinMode(syncPin, OUTPUT);
  pinMode(syncTrigPin, INPUT);
  digitalWrite(syncPin, LOW);
}

void loop() {
  if (syncOn == true) {
    // trigger at 500Hz
    if (sinceOnsetTest >= 2000){
//    if ((sinceOnsetTest >= (2000 + (5000000 - (counter *10)))) & (counter<500000)) {
      sinceOnsetTest = 0;
      SendPulse();
//      counter += 1;
//      digitalWriteFast(syncPin, !digitalReadFast(syncPin));
//      digitalWriteFast(syncPin, HIGH);
    } else {
//      digitalWriteFast(syncPin, LOW);
      TestSync();
    }
  } else {
    TestSync();
  }
}

void SendPulse() {
  pulseDuration=0;
  while (pulseDuration < 500) {
    digitalWriteFast(syncPin, HIGH);
  }
  digitalWriteFast(syncPin, LOW);
}

void TestSync() {
if (digitalReadFast(syncTrigPin) == HIGH) {
      syncOn = true;
    } else {
      syncOn = false;
    }
}
