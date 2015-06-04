// Pin Mappping:
// Pin 2: Rotary Encoder PinA
// Pin 3: Rotary Encoder PinB
// Pin 4: Heating Pad
// Pin 5: Buzzer
// Pin 6: LCD pin3 (contrast)
// Pin 7: Sensor
// Pin 14(A0): LCD pin 4
// Pin 15(A1): LCD pin 6
// Pin 16(A2): LCD pin 11
// Pin 17(A3): LCD pin 12
// Pin 18(A4): LCD pin 13
// Pin 19(A5): LCD pin 14

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);

int rotEncA = 2; // Rotary Encoder pinA
int rotEncB = 3; // Rotary Encoder pinB
int LCD_contrast = 6; 


volatile float freq=10;
volatile boolean halfleft = false;      // Used in both interrupt routines
volatile boolean halfright = false;

float maxfreq = 100;
float minfreq = 1;

int m0 = 8;
int m1 = 4;
int m2 = 5;
int m3 = 6;
int m4 = 7;

int m =1;
int MODE = 1;
boolean OUT = LOW;

int modeButtonL = 10;
int modeButtonR = 9;

long lastButton;
long currentTime;
long lastTime;
float f =0;
float T;

void setup(void)
{
  pinInit();
  lcd.begin(16, 2);
  resetSolenoids();
  dispMode(m);
}


void loop(void)
{ 

  boolean rVal = digitalRead(modeButtonR);
  boolean lVal = digitalRead(modeButtonL);

  if (rVal == HIGH && (currentTime - lastButton)>200) {    
    if(MODE == 6){
      MODE = 0;
    } else{
      MODE++;
    }
    resetSolenoids();
    lastButton = currentTime;
  } else if(lVal == HIGH && (currentTime - lastButton)>200) {
 
    if(MODE == 0){
      MODE = 6;
    } else{
      MODE--;
    }
    resetSolenoids();
    lastButton = currentTime;
  }

/////// Display Frequency /////////
if(f != freq || m != MODE){
    f = freq;
    m = MODE;
    dispMode(m);
    lcd.setCursor(0,0);
    if(freq<10){
      lcd.print(" ");
    }
    if(freq<100){
      lcd.print(" ");
    }
    lcd.print(freq,0);
    lcd.print(" Hz  ");
}
////////////////////////////

//////////////////////////

  T = 1000/freq; // half-Period in microseconds
  currentTime = millis();

if(currentTime - lastTime >= T/2){
  OUT = !OUT;
    // 4-magnets
    if(MODE <5){
     pulse(OUT,MODE);
    }else{
     pulse2(OUT,MODE); 
    }
  lastTime = currentTime;
}

}


void resetSolenoids(){
 digitalWrite(m0,LOW); 
 digitalWrite(m1,LOW); 
 digitalWrite(m2,LOW); 
 digitalWrite(m3,LOW); 
 digitalWrite(m4,LOW); 
}

void pulse(boolean out, int mode){
  if(mode == 0){
    digitalWrite(m0,out);
  } else if(mode == 1){
    digitalWrite(m1,out);
  } else if(mode == 2){
    digitalWrite(m2,out);
  } else if(mode == 3){
    digitalWrite(m3,out); 
  } else if(mode == 4){
    digitalWrite(m4,out); 
  } 
}

void pulse2(boolean out, int mode){
  if(mode == 5){
    digitalWrite(m1,out);
    digitalWrite(m3,!out);
  } else if(mode == 6){
    digitalWrite(m2,out);
    digitalWrite(m4,!out); 
  }
}

void dispMode(int mode){
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("MODE: ");
  if(mode == 0){
   lcd.print("Single"); 
  }else if(mode == 1){
    lcd.setCursor(10,0);
    lcd.print("M1"); 
  }else if(mode == 2){
    lcd.setCursor(12,1);
    lcd.print("M2");
  }else if(mode == 3){
    lcd.setCursor(10,1);
    lcd.print("M3");
  }else if(mode == 4){
    lcd.setCursor(8,1);
    lcd.print("M4");
  }else if(mode == 5){
    lcd.setCursor(10,0);
    lcd.print("M1"); 
    lcd.setCursor(10,1);
    lcd.print("M3");
  }else if(mode == 6){
    lcd.setCursor(12,1);
    lcd.print("M2");
    lcd.setCursor(8,1);
    lcd.print("M4");
  }
    
}


//////////////////// Rotary Encoder Interrupts //////////////////
//http://home.online.no/~togalaas/rotary_encoder/
void isr_2(){                                              // Pin2 went LOW
  delay(1);                                                // Debounce time
  if(digitalRead(rotEncA) == LOW){                               // Pin2 still LOW ?
    if(digitalRead(rotEncB) == HIGH && halfright == false){      // -->
      halfright = true;                                    // One half click clockwise
    } 
    if(digitalRead(rotEncB) == LOW && halfleft == true){         // <--
      halfleft = false;      // One whole click counter-
      if(freq<maxfreq){
        freq+=1;                                            // clockwise
      }
    }
  }
}
void isr_3(){                                             // Pin3 went LOW
  delay(1);                                               // Debounce time
  if(digitalRead(rotEncB) == LOW){                              // Pin3 still LOW ?
    if(digitalRead(rotEncA) == HIGH && halfleft == false){      // <--
      halfleft = true;                                    // One half  click counter-
    }                                                     // clockwise
    if(digitalRead(rotEncA) == LOW && halfright == true){       // -->
      halfright = false;                                  // One whole click clockwise
      if(freq>minfreq){
        freq-=1; 
      }
    }
  }
}

void pinInit(){
    // Initialize LCD
  pinMode(16,OUTPUT); // LCD 11 to pin 16(A2)
  pinMode(17,OUTPUT); // LCD 12 to pin 17(A3)
  pinMode(18,OUTPUT); // LCD 13 to pin 18(A4)
  pinMode(19,OUTPUT); // LCD 14 to pin 19(A5)

  //Initialize Rotary Encoder
  pinMode(rotEncA, INPUT);
  digitalWrite(rotEncA, HIGH);                // Turn on internal pullup resistor
  pinMode(rotEncB, INPUT);
  digitalWrite(rotEncB, HIGH);                // Turn on internal pullup resistor
  attachInterrupt(0, isr_2, FALLING);   // Call isr_2 when digital pin 2 goes LOW
  attachInterrupt(1, isr_3, FALLING);   // Call isr_3 when digital pin 3 goes LOW
  // Initialize Solenoids
  pinMode(m0,OUTPUT); 
  pinMode(m1,OUTPUT); 
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT); 
  pinMode(m4,OUTPUT); 

  pinMode(modeButtonL,INPUT);
  pinMode(modeButtonR,INPUT);
  Serial.begin(9600);
}

