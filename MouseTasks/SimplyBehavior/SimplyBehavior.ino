/* 
No task - just behavior
*/

// intended for use with Bonsai workflow  (default: RunBehavior_Video.bonsai)
// includes a function to send data to the PC
const int TTLPin = 8;

byte SessionStatus[2] = {0, 0}; // 1/ Run [ON/OFF (1/0)] 2/ Reset (1)
byte TrialType = 0; // 0, no trial going on; 1 left trial; 2, right trial // left or right doesn't matter here
int TrialCount=0;
int TrialInit=0;

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

//=============

void setup() {
 Serial.begin(9600); // Bonsai baudrate
 randomSeed(analogRead(A4));
    
pinMode(TTLPin, OUTPUT);
}

//=============

void loop() {

  // curMillis = millis();
  getDataFromPC();

  if(SessionStatus[1] == 1){ // reset counters
    TrialCount=0;
  }

//  while (SessionStatus[0] == 0){ 
//    getDataFromPC();
//  }

  if (SessionStatus[0] == 1 && TrialInit==0){
          TrialCount=TrialCount+1;
          TTLout(1);
          if (TrialCount==1){
          Serial.println("Start"); // this will be skiped by serial read
          }
          sendToPC(TrialCount,TrialType);
          TrialInit=1;
          delay(100);
    }

    if (SessionStatus[0] == 0 && TrialInit==1){
          TTLout(2);
//          Serial.println("End");
          sendToPC(TrialCount,0); // end current trial
          TrialInit=0;
          delay(100);
    }
//delay(100);
}

//=============

void getDataFromPC() {
//  sendToPC(9,9,9);
// receive data from PC and save it into inputBuffer
  if(Serial.available() > 0) {
    char x = Serial.read();
//    Serial.println(x);
      // the order of these IF clauses is significant
      
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
//      Serial.println("End Marker");
//      sendToPC(SessionStatus[0],SessionStatus[1],TrialType);
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
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  SessionStatus[1] = atoi(strtokIndx);
  
  strtokIndx = strtok(NULL, ","); 
  TrialType = atoi(strtokIndx)+1;
}

//=============

void sendToPC(int TrialCount, byte TrialType) {
  Serial.print(TrialCount);
  Serial.print(",");
  Serial.print(TrialType);
  Serial.println(",");
}

//=============

void TTLout(int instruct){
  digitalWrite(TTLPin, HIGH);
  delay(1000);
  digitalWrite(TTLPin, LOW);
switch (instruct) {
  case 1: 
    // trial initiation: 2 TTL
    delay(1000);
    digitalWrite(TTLPin, HIGH);
    delay(1000);
    digitalWrite(TTLPin, LOW);
    break;
  case 2:
  // that's it: 1 TTL pulse is enough
    break;
  }
}

