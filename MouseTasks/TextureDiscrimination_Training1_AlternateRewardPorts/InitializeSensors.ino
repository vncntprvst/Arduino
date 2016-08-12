// see also runningMedian Class for Arduino: http://playground.arduino.cc/Main/RunningMedian
 void arrayinit(){
  int  inc;
  int Ltotal;
  int Rtotal;
  int F1total;
  int F2total;
  float LeftIRvalArray[4];
  float RightIRvalArray[4];
  float FrontIR1valArray[4];
  float FrontIR2valArray[4];
  Serial.println(" ");
  Serial.println("Start IR initialization");
  while (Ltotal!=3 && Rtotal!=3 && F1total!=3 && F2total!=3){
    Ltotal=0;
    Rtotal=0;
    F1total=0;
    F2total=0;
    for (inc=0; inc < 4 ; inc++ ){
      LeftIRvalArray[inc] = analogRead(LeftIRread);
      RightIRvalArray[inc] = analogRead(RightIRread);
      FrontIR1valArray[inc] = analogRead(FrontIR1read);
      FrontIR2valArray[inc] = analogRead(FrontIR2read);
      if ((inc>0) && LeftIRvalArray[inc]==LeftIRvalArray[inc-1]){
        Ltotal++;
      }   
      if ((inc>0) && RightIRvalArray[inc]==RightIRvalArray[inc-1]){
        Rtotal++;
      }   
      if ((inc>0) && FrontIR1valArray[inc]==FrontIR1valArray[inc-1]){
        F1total++;
      }  
      if ((inc>0) && FrontIR2valArray[inc]==FrontIR2valArray[inc-1]){
        F2total++;
      }  
      delay(10);  
      Serial.print("Left IR val ");
      Serial.print((long)LeftIRvalArray[inc], DEC); 
      Serial.print(" & Right IR val "); 
      Serial.println((long)RightIRvalArray[inc], DEC);
      Serial.print(" & Front IR 1 val "); 
      Serial.print((long)FrontIR1valArray[inc], DEC);
      Serial.print(" & Front IR 2 val "); 
      Serial.println((long)FrontIR2valArray[inc], DEC);
    }
  }
  Lbaseline=LeftIRvalArray[3];
  Rbaseline=RightIRvalArray[3];
  F1baseline=FrontIR1valArray[3];
  F2baseline=FrontIR2valArray[3];
  Serial.print("Left Baseline value is ");
  Serial.print(Lbaseline);
  Serial.print(" and Right Baseline value is ");
  Serial.println(Rbaseline);
  Serial.print(" and Front Baseline values are ");
  Serial.print(F1baseline);
  Serial.print(" and ");
  Serial.println(F2baseline);
  Serial.println("End initialization");
  // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}
