// Trigger signal for High-speed camera

IntervalTimer pulseTimer;

const int syncPin = 0;
const int syncTrigPin = 1;
//elapsedMicros sinceOnsetTest;
elapsedMicros pulseDuration;
//bool syncOn = true; //true

void setup() {
  pinMode(syncPin, OUTPUT);
  pinMode(syncTrigPin, INPUT);
  digitalWrite(syncPin, LOW);
  pulseTimer.begin(SendPulse, 2000); // pulse avery 2ms
}

void loop() {
}

void SendPulse() {
  if (digitalReadFast(syncTrigPin) == HIGH) {
    pulseDuration=0;
    while (pulseDuration < 1000) {
      digitalWriteFast(syncPin, HIGH);
    }
    digitalWriteFast(syncPin, LOW);
  }
}
