/* Reading IR phototransistors */

const int LeftIRread = A0; 
const int RightIRread = A1;  
const int FrontIR1read = A2;
const int FrontIR2read = A3;  
long Lbaseline = 0;
long Rbaseline = 0;
long F1baseline = 0;
long F2baseline = 0;
long LeftIRval = Lbaseline;           // store read value
long RightIRval = Rbaseline; 
long FrontIR1val = F1baseline;
long FrontIR2val = F2baseline;

int Lrewtrig=0;
int Rrewtrig=0;
int TopFrewtrig=0;
int BottomFrewtrig=0;

void setup()
{
  Serial.begin(9600);          //  setup serial
// arrays initialization
    arrayinit();  
}

void loop()
{

  LeftIRval = analogRead(LeftIRread);    
  RightIRval = analogRead(RightIRread); 
//  Serial.print("Left IR ");
//  Serial.print(LeftIRval);
//  Serial.print(" and ");
//  Serial.print(" Right IR ");  
//  Serial.print(RightIRval);
//  Serial.println(" ; ");
  
  FrontIR1val = analogRead(FrontIR1read); // read the input pin
  FrontIR2val = analogRead(FrontIR2read); // read the input pin
  Serial.print("Front IRs ");
  Serial.print(FrontIR1val);
  Serial.print(" and ");
  Serial.println(FrontIR2val);

        
    if (FrontIR1val > F1baseline + 100){
      BottomFrewtrig=BottomFrewtrig+1;
    }
    if (FrontIR2val > F2baseline + 100){
      TopFrewtrig=TopFrewtrig+1;
    }
    if (LeftIRval > Lbaseline + 200){
      Lrewtrig=Lrewtrig+1;
    }
    if (RightIRval > Rbaseline + 200){
      Rrewtrig=Rrewtrig+1;
    }

    if ((BottomFrewtrig>5) || (TopFrewtrig>5) || (Lrewtrig>5) || (Rrewtrig>5)){
      Serial.print(" Detected ");
      if (Lrewtrig>5){
        Serial.print(" Left IR");
        Lrewtrig=0;
      }
      if (Rrewtrig>5){
        Serial.print(" Right IR");
        Rrewtrig=0;
      }
      if (BottomFrewtrig>5){
        Serial.print(" Front IR (BOTT)");
        BottomFrewtrig=0;
      }
      if (TopFrewtrig>5){
        Serial.print(" Front IR (TOP)");
        TopFrewtrig=0;
      }
      Serial.println(".");
    }
        
  delay(200);
}

void arrayinit(){
  int  inc;
  int Ltotal=0;
  int Rtotal=0;
  int F1total=0;
  int F2total=0;
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
  Lbaseline=(long)LeftIRvalArray[3];
  Rbaseline=(long)RightIRvalArray[3];
  F1baseline=(long)FrontIR1valArray[3];
  F2baseline=(long)FrontIR2valArray[3];
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
