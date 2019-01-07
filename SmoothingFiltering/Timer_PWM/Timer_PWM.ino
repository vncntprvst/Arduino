// Testing fast PWM, using Timer2
// Used with RC filter like 470Ohm 0.22uF anf 5ms pulses. 
// output is sine/trapezoidal but filter doesn't smooth out initial part of the curve 

//see:  https://playground.arduino.cc/Main/TimerPWMCheatsheet
//      https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
// example for Audio: http://playground.arduino.cc/Code/PCMAudio

//Pins 11 and 3: controlled by timer 2 in phase-correct PWM mode (cycle length = 510)
//  Timer output    Arduino output 
//  OC2A            11
//  OC2B            3
//  Setting   Divisor   Frequency
//  0x01    1     31372.55
//  0x02    8     3921.16
//  0x03      32      980.39
//  0x04    64    490.20   <--DEFAULT
//  0x05    128     245.10
//  0x06      256     122.55
//  0x07    1024      30.64
//
//TCCR2B = (TCCR2B & 0b11111000) | <setting>;
//Timer2: 8 bit
//#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
//#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

const int TTL_Pin = 11;
const float pi = 3.14;
//loop counters
short  rampPos = 0;      
long  plateauCount = 0;
//byte  rampDownPos = 0;
bool  rampDirection = true;
int outputVal=0;

void setup() {
    Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TTL_Pin, OUTPUT);
//  TCCR2B = (TCCR2B & 0b11111000) | 0x01;

// Initialize Timer2
  TCCR2A = 0;
  TCCR2B = 0;

// Set mode 5, PWM phase correct mode, counts up and down
  bitSet(TCCR2A, WGM20);
  bitSet(TCCR2B, WGM22);

// Set up 1 prescale
  bitSet(TCCR2B, CS20);

// Set frequency 36kHz (16MHz / 1 / 222 / 2 (because of PWM Phase Correct) = 36 036)
  OCR2A = 222; //'TOP' value at which the timer starts counting down
  OCR2B = 111; // When the timer is below OCR2B the output pin is HIGH

//  cbi (TIMSK0,TOIE0);              // disable Timer0 !!! delay() is now not available
//  sbi (TIMSK2,TOIE2);              // enable Timer2 Interrupt

}

// Check that changing the timer preset on pins 11 and 3 doesn't impact millis and delay
// just blink LED to test delay 
void loop() {
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
//  digitalWrite(TTL_Pin, HIGH);   // trigger TTL
//  delay(5);                       // wait
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  digitalWrite(TTL_Pin, LOW);   
//  delay(100);                       // wait 




  if ((rampPos < 25) && (rampDirection == true)) {
    rampPos++; 
  }
  else if ((rampPos <= 0) && (rampDirection == false)) {
//    digitalWrite(TTL_Pin, LOW);   
    while (plateauCount < 1000000){
      plateauCount++;
    }
    plateauCount=0;
    rampDirection = true;
  }
  else if ((rampPos < 25) && (rampDirection == false)) {
    rampPos--;
  }
  else if (rampPos >= 25) {
    while (plateauCount < 1000000){
      plateauCount++;
    }
    plateauCount=0;
    rampDirection = false;
    rampPos=24;
  }
  
  outputVal = 255*(1-cos(rampPos*(pi/2)/25));
  analogWrite(TTL_Pin, outputVal); 
  Serial.println(255*(1-cos(rampPos*(pi/2)/25)));
}
//    delay(5);
//    analogWrite(TTL_Pin, 0);
//    delay(1);
//    analogWrite(TTL_Pin, 51);
//    delay(1);
//    analogWrite(TTL_Pin, 102);
//    delay(1);
//    analogWrite(TTL_Pin, 153);
//    delay(1);
//    analogWrite(TTL_Pin, 204);
//    delay(1);
//    analogWrite(TTL_Pin, 255);
//    delay(1);

