void getDataFromPC() {
// receive data from PC and save it into inputBuffer
  if(Serial.available() > 0) {
    char x = Serial.read();
      // the order of these IF clauses is significant

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
  
  strtokIndx = strtok(inputBuffer,","); // get the first part (session ON / OFF)
  SessionStatus[0] = atoi(strtokIndx); //  convert to an integer
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  SessionStatus[1] = atoi(strtokIndx); // SessionStatus[1] is to reset counters
  
  strtokIndx = strtok(NULL, ","); 
//    if (strtokIndx=="True"){
//    TrialType = 1;
//  } else if (strtokIndx=="False"){
//    TrialType = 2;
// }
  TrialType = atoi(strtokIndx)+1;

  if (TrialBlockMode==1){ // ignore trial type given by PC
    if (TrialCount==0){ //just starting -> set trial type accordingly
      TrialType = 1;
    }
  }
}
