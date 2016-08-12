void rewardflush(){
  Serial.println("flush");
  // flush left
  LeftSolenoid->run(FORWARD);
  LeftSolenoid->setSpeed(255);
  delay(1500);
  LeftSolenoid->run(RELEASE);
  // flush right
  RightSolenoid->run(FORWARD);
  RightSolenoid->setSpeed(255);
  delay(1500);
  RightSolenoid->run(RELEASE);
}
