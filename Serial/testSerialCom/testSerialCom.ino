
byte SessionStatus[2] = {0, 0}; // 1/ Run [ON/OFF (1/0)] 2/ Reset (1)
byte TrialType = 0; // 0, no trial going on; 1 left trial; 2, right trial // left or right doesn't matter here

// GUI-related variables
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};

unsigned long curMillis;

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 3500;

int TrialCount=0;

// sound stimulus
//const int SoundTriggerPin=11;

//=============

void setup() {
 Serial.begin(9600);
 
//pinMode(SoundTriggerPin, OUTPUT);

pinMode(13, OUTPUT);
}

//=============

void loop() {

  // curMillis = millis();
//  Serial.println("Start");
  getDataFromPC();

  if(SessionStatus[1] == 1){ // reset counters

  }

  while (SessionStatus[0] == 0){ 
//    Serial.println("Ready");
    getDataFromPC();
//    delay(500);
//    Serial.println("Still");
    }

  if (SessionStatus[0] == 1){ 
    //  Serial.println(lightValue);
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(250);  
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(250); 
//  TTLout(2);
  TrialCount=TrialCount+1;
  if (TrialCount==1){
  Serial.println("Start");
  Serial.println("trial result, succes count, time ");
  }
  sendToPC(2,TrialCount);
  getDataFromPC();
  }
}
//=============

void getDataFromPC() {
// receive data from PC and save it into inputBuffer
  if(Serial.available() > 0) {
    char x = Serial.read();
      // the order of these IF clauses is significant
//    Serial.print("got data ");
//    Serial.println(x);
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}

//=============
 
void parseData() {

    // split the data into its parts
    // assumes the data will be received as (eg) 1,0,2
    
  char * strtokIndx; // this is used by strtok() as an index
  
  

  strtokIndx = strtok(inputBuffer,","); // get the first part
  SessionStatus[0] = atoi(strtokIndx); //  convert to an integer
//  Serial.println(strtokIndx);
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  SessionStatus[1] = atoi(strtokIndx);
//  Serial.println(strtokIndx);
  
  strtokIndx = strtok(NULL, ","); 
  if (strtokIndx=="True"){
    TrialType = 1;
  } else if (strtokIndx=="False"){
    TrialType = 2;
 }
 
 if (SessionStatus[0] == 1){
 Serial.println("Start running");
 }
 
}

//=============

void sendToPC(int trial_result, int success_trial_count) {
	// if (curMillis - prevReplyToPCmillis >= replyToPCinterval) {
	// 	prevReplyToPCmillis += replyToPCinterval;
	// 	int valForPC = curMillis >> 9; // approx half seconds
	// 	Serial.print('<');
	// 	Serial.print(valForPC);
	// 	Serial.print('>');
	// }

  Serial.print(trial_result);
  Serial.print(",");
  Serial.print(success_trial_count);
  Serial.println(",");

}

//=============
//
//void TTLout(int instruct){
////HIGH triggers trinkets listening
//Serial.println("TTL out");
//switch (instruct) {
//    // both cases should total 10ms
//    case 1: 
//    // white noise
//      digitalWrite(SoundTriggerPin, HIGH); // trigger
//// NB: 
//// if control PlayTone enabled in WhiteNoise_USbeep
//// need to add a total of 100ms (or control beep's duration)
//// to wait for if statment 
//      delay(2); 
//      digitalWrite(SoundTriggerPin, LOW); // White noise
//      delay(8);
//      break;
//    case 2:
//      // US
//      digitalWrite(SoundTriggerPin, HIGH); // trigger
//      delay(2); 
//      digitalWrite(SoundTriggerPin, LOW);
//      delay(2);
//      digitalWrite(SoundTriggerPin, HIGH); // +1
//      delay(2);
//      digitalWrite(SoundTriggerPin, LOW);
//      delay(4);
//      break;
//  }
//}
