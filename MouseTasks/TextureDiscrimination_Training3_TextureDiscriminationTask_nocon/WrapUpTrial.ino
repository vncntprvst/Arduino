void trialwrapup(int timeout){ // needs to happen after sendToPC() command
  if (TrialBlockMode==1){ // time to move trial position one up (ie, setting up next trial type)
    BlockPos+=1;
    if (BlockPos<=BlockSize){
      TrialType=1;
    } else if ((BlockPos>BlockSize) && (BlockPos<=2*BlockSize)){
      TrialType=2;
    }
    if (BlockPos==2*BlockSize){
      BlockPos=0;
    }
//    Serial.print("End of trial. BlockPos moved to ");
//    Serial.println(BlockPos);
//    Serial.print("TrialType = ");
//    Serial.println(TrialType);
  }
  SoundOut(1); // white noise mask 
  panelrotate(); // set for next trial
//      Serial.print("Reward count: ");
//      Serial.println(RewCount);
  Lrewtrig=0;
  Rrewtrig=0;
  TopFrewtrig=0;
  BottomFrewtrig=0;
  LeftGLight=0;
  RightGlight=0;
  resetfp=1;
  TrialInit=0;
  // refractory period
  delay(timeout); // timeout to leave time for 2s white noise + extra timeout if wrong trial 
}
