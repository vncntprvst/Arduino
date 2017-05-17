int next_pos;
int type1;
int type2;
int type3;
int type4;
int type5;

void setup() {
  Serial.begin(9600);
}

void loop() {
  next_pos = random(1,6);
//  Serial.println(next_pos);
  switch(next_pos){
    case 1:
      type1++; 
      break;
    case 2:
      type2++; 
      break;
    case 3:
      type3++;
      break; 
    case 4:
      type4++;
      break; 
    case 5:
      type5++; 
      break;
  }
  
Serial.print(type1);
Serial.print(" ");
Serial.print(type2);
Serial.print(" ");
Serial.print(type3);
Serial.print(" ");
Serial.print(type4);
Serial.print(" ");
Serial.println(type5);
//delay(5);
}
