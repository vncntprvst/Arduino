/* Reading 2 IR phototransistors
VP - 4/11/2014 */

const int analogPin1 = A2;
const int analogPin2 = A3;   
int baseline =0;
int val = baseline;           // store read value

void setup()
{
  Serial.begin(9600);          //  setup serial
// arrays initialization
//    arrayinit();  
}

void loop()
{
  val = analogRead(analogPin1);    // read first input pin
  Serial.print("First sensor : ");   
  Serial.print(val);   
  val = analogRead(analogPin2);    // read 2nd input pin
  Serial.print(" .Second sensor : ");    
  Serial.println(val);   
  delay(200);
}

void arrayinit(){
  int  inc;
  int total;
  float IRvalArray[3];
  
  Serial.println(" ");
  Serial.println("Start IR initialization");
  while (total!=3){
  total=0;
  for (inc=0; inc < 4 ; inc++ ){
    IRvalArray[inc] = analogRead(analogPin1);
    if ((inc>0) && IRvalArray[inc]==IRvalArray[inc-1]){
      total++;
    }   
  delay(10);  
//  Serial.println((long)IRvalArray[inc], DEC);  
  }
  }
  baseline=IRvalArray[3];
  Serial.print("Baseline value is ");
  Serial.println(baseline);

  Serial.println("End initialization");
}
