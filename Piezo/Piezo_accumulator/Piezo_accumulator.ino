// Adapted from https://davidhoulding.blogspot.com/2014/11/advanced-high-sensitivity-vibration.html

#define piezoInput A0
#define vib_arraySize 100

#define vibThd 1
#define vib_eventThd 3
#define vib_eventPeriod 500

long vibrationEvents[vib_arraySize];
int vibrationEventIndex;

void setup(){
  Serial.begin(19200);  
  vibrationEventIndex = 0;
  clearVibrationEvents();
}

void loop(){
  if(analogRead(piezoInput) > 0){
    long currentMillis = millis();
    addVibrationSample(currentMillis);
    if(isVibrationAlarmTiggered()){
      Serial.println(String(millis()) + "\t ALARM");
      delay(200);
    }
//    delay(1); 
  }
}

void addVibrationSample(long vibrationMillis){
   vibrationEvents[vibrationEventIndex++] = vibrationMillis;
   if(vibrationEventIndex >= vib_arraySize){
     vibrationEventIndex = 0; // wrap vibration sample index around when we get to end of sample array
   }
}

void clearVibrationEvents(){
  for(int i = 0; i < vib_arraySize ; ++i){
    vibrationEvents[i] = -1;
  }
}

boolean isVibrationAlarmTiggered(){
  long thresholdMillis = millis() - vib_eventPeriod;
  if(thresholdMillis < 0) thresholdMillis = 0;
  int numVibrationsSinceThreshold = 0;
  for(int i = 0; i < vib_arraySize ; ++i){
    if(vibrationEvents[i] >= thresholdMillis){
      ++numVibrationsSinceThreshold;
    }
  }
  
//  Serial.println(String(millis()) + "\t# events: " + String(numVibrationsSinceThreshold));
  boolean alarmTriggered = false;
  if(numVibrationsSinceThreshold >= vib_eventThd){
    alarmTriggered = true;
  }
  
  return alarmTriggered;
}

