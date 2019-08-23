// Trigger signal for High-speed camera
const int syncPin = 0;
const int syncTrigPin = 1;
elapsedMicros sinceTest1;
//int counter = 0;
//bool syncOn = true; //false

void setup() {
  pinMode(syncPin, OUTPUT);
  pinMode(syncTrigPin, INPUT);
  digitalWrite(syncPin, LOW);
}

void loop() {
  //  if (syncOn == false) {
  //  if (digitalReadFast(syncTrigPin) == HIGH) {
  //    syncOn = true;
  //  } else {
  //    syncOn = false;
  //  }
  //  }
  //  if (syncOn == true) {
  //    if (counter<3000) {
  //     trigger at 500Hz
  if (sinceTest1 >= 1000) {
  digitalWriteFast(syncPin, HIGH);
  delay(1);              // 1ms ON phase duration
  digitalWriteFast(syncPin, LOW);
  delay(1);              // 1ms interval between pulses
  //      counter+=1;
  //    }
  //  }
}
