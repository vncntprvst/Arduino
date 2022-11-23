/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
int LEDpin = 12;
int BNC_LED = 8;
char firethelaser;
int count = 0;
unsigned long pulseTime;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);
  pinMode(BNC_LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop(){
  if (Serial.available()>0){
    
    firethelaser = Serial.read(); // read user input
  
      if (firethelaser == 'G'){
        while (count < 200){
        digitalWrite(LEDpin, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(BNC_LED, HIGH);   
        delay(5);// pulse width
        digitalWrite(BNC_LED, LOW);
        delay(50);  // interpulse interval
        count += 1;
        pulseTime = millis();
        Serial.print(count);
        Serial.print(",");
        Serial.println(pulseTime);
        }
      count = 0;
      digitalWrite(LEDpin, LOW);
      }
  }
    // wait for a second
}
