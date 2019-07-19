// Trigger signal for High-speed camera
const int SyncPin = 0;
int counter = 0;

void setup() {
  pinMode(SyncPin, OUTPUT);
  digitalWrite(SyncPin, LOW); 
}

void loop() {
//  if (counter<3000) {
//     trigger at 500Hz
    digitalWrite(SyncPin, HIGH);   
    delay(1);              // 1ms ON phase duration 
    digitalWrite(SyncPin, LOW);  
    delay(1);              // 1ms interval between pulses 
//    counter+=1;
//  }
}
