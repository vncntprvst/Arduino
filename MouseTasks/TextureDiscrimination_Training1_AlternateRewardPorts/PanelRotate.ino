void panelrotate(){
  // Rotate panel for next trial
  if (TrialSelectMode==0){ // random rotation
    next_pos = random(1,201); // random number between 1 and 200 (included)
    if (next_pos<101){
      TrialType=1;
      // Serial.print("panelrotate says TrialType is ");
      // Serial.println(TrialType);
    } else {
      TrialType=2;
      // Serial.print("panelrotate says TrialType is ");
      // Serial.println(TrialType);
    }
//   else if (TrialSelectMode==1)  //instructions from computer
  } else if (TrialSelectMode==2) {
      if (TrialType==1){// current trial within first block type
         next_pos = random(1,101);
    } else if (TrialType==2){
         next_pos = random(101,201); 
    }
  } else if (TrialSelectMode==3) {
      if (curr_pos < 101){// current trial within first block type
         next_pos = random(101,201);
         TrialType=2;
    } else if (curr_pos > 100){
         next_pos = random(1,101); 
         TrialType=1;
    }
  } 
  
//  Serial.print("panelrotate next pos ");
//  Serial.println(next_pos);
  rot_seq = random(200); // random number between 0 and 1
//  Serial.print("panelrotate rot seq ");
//  Serial.println(rot_seq);
  
  if (((curr_pos < 101) && (next_pos < 101)) || ((curr_pos > 100) && (next_pos > 100))){ 
    if (rot_seq < 100 ) { 
//      Serial.println("Rotate CW 1/4");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
      delay(100);
//      Serial.println("Rotate CCW 1/4");
     TexturePanelStepper->step(50, BACKWARD, DOUBLE); 
    } else {
//      Serial.println("Rotate CCW 1/4");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      delay(100);
//      Serial.println("Rotate CW 1/4");
      TexturePanelStepper->step(50, FORWARD, DOUBLE); 
    }
  } else { //present different texture
    if (rot_seq < 100 ) {
//      Serial.println("Rotate CCW 1/4");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE); 
      delay(100);
//      Serial.println("Rotate CCW 1/4");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE); 
    } else {
//      Serial.println("Rotate CW 1/4");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
      delay(100);
//      Serial.println("Rotate CW 1/4");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
    }
  }
//    TexturePanelStepper->release();
  curr_pos = next_pos;
  // Serial.print("curr_pos is now ");
  // Serial.println(curr_pos);
}
