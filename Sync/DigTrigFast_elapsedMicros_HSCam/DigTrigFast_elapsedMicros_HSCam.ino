// Trigger signal for High-speed camera
const int syncPin = 0;
const int syncTrigPin = 1;
elapsedMicros sinceOnsetTest;
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
    if (sinceOnsetTest >= 1000) {
      sinceOnsetTest = sinceOnsetTest - 1000;
      digitalWriteFast(syncPin, HIGH);
    } else {
      digitalWriteFast(syncPin, LOW);
      TestSync();
    }
  } else {
    TestSync();
  }
}

void TestSync() {
if (digitalReadFast(syncTrigPin) == HIGH) {
      syncOn = true;
    } else {
      syncOn = false;
    }
}
