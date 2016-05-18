
// 4/19/16 two pots: one controls angle, the other the delay
//things go astray around 25 degrees and 2ms delay

  #include <Servo.h> 
   
  int anglePin = 0;  
  int delayPin = 1;
  int servoPin = 9;
  int setAngle = 0; // starting position
  int angle = 0;
  int setDelay = 1;
  Servo stimServo; 
   
  void setup() 
  { 
    Serial.begin(9600); 
    stimServo.attach(servoPin);  
    stimServo.write(setAngle);
  } 
   
  void loop() 
  { 
    int reading = analogRead(anglePin);     // 0 to 1023
    setAngle = round(reading / 20) + 5;              // 5 to 56
    setDelay = round(1023/analogRead(delayPin));
    if (setDelay>20){
      setDelay=20;
    }
    Serial.print("target angle: ");
    Serial.println(setAngle);
//    Serial.print("step delay: ");
//    Serial.println(setDelay);
    Serial.print("target frequency: ");
    Serial.println(1000/(2*setDelay*setAngle));
//    delay(200);
      for(angle = 0; angle < setAngle; angle++)  
      {                                  
        stimServo.write(angle);       
        delay(setDelay);                   
      } 
      for(angle = setAngle; angle > 0; angle--)    
      {                                
        stimServo.write(angle);           
        delay(setDelay);       
      }
  } 
