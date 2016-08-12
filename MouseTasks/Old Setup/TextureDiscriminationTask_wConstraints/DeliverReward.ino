void reward(Adafruit_DCMotor* solenoid,int dur){
  solenoid->setSpeed(255);  
  solenoid->run(FORWARD);
//    for (int dec=200; dec>170; dec-=decrease) { //max 255
//    solenoid->setSpeed(dec);
//    delay(15);
//    Serial.println(dec);
//   }
 delay(dur);
 solenoid->run(RELEASE); // cut power to motor
}
