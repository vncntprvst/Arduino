/* Reading dual IR phototransistor (which is just the same as one, I know)
VP - 2/3/16 */

const int analogPin = A2;  
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
  val = analogRead(analogPin);    // read the input pin
  Serial.println(val);   
  delay(200);
}

//void arrayinit(){
//  int  inc;
//  int total;
//  float IRvalArray[3];
//  
//  Serial.println(" ");
//  Serial.println("Start IR initialization");
//  while (total!=3){
//  total=0;
//  for (inc=0; inc < 4 ; inc++ ){
//    IRvalArray[inc] = analogRead(analogPin);
//    if ((inc>0) && IRvalArray[inc]==IRvalArray[inc-1]){
//      total++;
//    }   
//  delay(10);  
//  Serial.println((long)IRvalArray[inc], DEC);  
//  }
//  }
//  baseline=IRvalArray[3];
//  Serial.print("Baseline value is ");
//  Serial.println(baseline);
//
//  Serial.println("End initialization");
//}
