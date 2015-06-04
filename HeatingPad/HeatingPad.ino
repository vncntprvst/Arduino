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

// This Arduino sketch reads DS18B20 "1-Wire" digital
// temperature sensors.
// Tutorial:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-tutorial.html

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
// Data wire is plugged into pin 0 on the Arduino
#define ONE_WIRE_BUS 7
int heatingPad = 4; // Heating pad on pin 4
int rotEncA = 2; // Rotary Encoder pinA
int rotEncB = 3; // Rotary Encoder pinB
int LCD_contrast = 6; 
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress sensor = {0x28, 0x83, 0x66, 0x74, 0x06, 0x00, 0x00, 0x57};

float minTemp = 25;
float maxTemp = 50;

float temp;
long onTime=0;
long startOn=0;
long lastOn;
long currentOn;
long lastOff=0;
long currentOff=0;
long interval;
boolean initialSet = false;
boolean on = false;

volatile float setPoint = 37;  
volatile boolean halfleft = false;      // Used in both interrupt routines
volatile boolean halfright = false;

void setup(void)
{
  // Initialize LCD
  pinMode(16,OUTPUT); // LCD 11 to pin 16(A2)
  pinMode(17,OUTPUT); // LCD 12 to pin 17(A3)
  pinMode(18,OUTPUT); // LCD 13 to pin 18(A4)
  pinMode(19,OUTPUT); // LCD 14 to pin 19(A5)
  analogWrite(LCD_contrast,100); /// Set LCD contrast with PWM on pin 6
  lcd.begin(8, 2);
  
  // Initialize Sensor
  sensors.begin();
  sensors.setResolution(sensor, 12);// set the resolution to 12 bit
  
  //Initialize Rotary Encoder
  pinMode(rotEncA, INPUT);
  digitalWrite(rotEncA, HIGH);                // Turn on internal pullup resistor
  pinMode(rotEncB, INPUT);
  digitalWrite(rotEncB, HIGH);                // Turn on internal pullup resistor
  attachInterrupt(0, isr_2, FALLING);   // Call isr_2 when digital pin 2 goes LOW
  attachInterrupt(1, isr_3, FALLING);   // Call isr_3 when digital pin 3 goes LOW
  // Initialize Heating Pad
  pinMode(heatingPad,OUTPUT);  

}

void loop(void)
{ 
  sensors.requestTemperatures();
  temp = sensors.getTempC(sensor);

  // Once initially heated up, if the heater is on
  // for more than 20 seconds, display warning and
  // sound alarm to check sensor to avoid burning
  onTime = millis() - startOn;
  if(on && initialSet && (onTime > 30000)){
    //tone(5,440);
    lcd.setCursor(0,0);
    //lcd.print("WARNING");
  }else{
    noTone(5);
    lcd.setCursor(0,0);
    lcd.print(">");
    lcd.print(setPoint,1);
    lcd.print("   ");
  }
  

  lcd.setCursor(0,1);
  lcd.print(" ");
  lcd.print(temp,1);
  
  if(temp>=setPoint+0.5){
    if(on && !initialSet){
      initialSet = true;
    }
    on = false;
    digitalWrite(heatingPad,LOW);
  }else if (temp<setPoint+0.5){
    if(!on){
     startOn = millis();
     lastOn = startOn;
     on = true; 
    }
     interval = 500;
     if(((setPoint+0.5-temp) <= 3.5) && ((setPoint+0.5-temp) > 2.5)){
       interval = 1000;
     }else if(((setPoint+0.5-temp) <= 2.5) && ((setPoint+0.5-temp) > 1.5)){
       interval = 1500;
     } else if(((setPoint+0.5-temp) <= 1.5) && ((setPoint+0.5-temp) > 0.5)){
       interval = 2000;
     } else if(((setPoint+0.5-temp) <= 0.5) && ((setPoint+0.5-temp) > 0.3)){
       interval = 5000;
     }else if((setPoint+0.5-temp)<=0.3){
        interval = 8000;
      }
    currentOn = millis();
    if((currentOn - lastOn) <= 1500){
          digitalWrite(heatingPad,HIGH);
          lcd.print(" *");
    }else if((currentOn-lastOn )> 1500 && (currentOn-lastOn) < 1500 + interval){
          digitalWrite(heatingPad,LOW);
           lcd.print("  ");
    }else{
          lastOn = currentOn; 
    }

        
        
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
      if(setPoint>minTemp){
        setPoint+=0.1;                                            // clockwise
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
      if(setPoint<maxTemp){
        setPoint-=0.1; 
      }
    }
  }
}

