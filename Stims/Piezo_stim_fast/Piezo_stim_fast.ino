//Total duration = 20 seconds

uint32_t trial_duration = 4500;
uint32_t iti_duration = 15500;
uint32_t stim_on = 1500;
uint32_t stim_off = 3000;
uint32_t piezo_period_ms = 100;
uint32_t piezo_stim_duration_ms = 5;
uint32_t bluePin = 41;
uint32_t greenPin = 33;
uint32_t piezoPin = 29;
uint32_t cameraPin = 22;

uint32_t frame_duration_ms = 10;
uint32_t frame_period_ms = 60;
uint32_t total_frames = 0;
uint32_t LED_period_ms = 20;
uint32_t total_LED = 0;
uint32_t last_piezo_switch = 0;
uint32_t start_trial = 0;
uint32_t start_iti = 0;
uint32_t time_since_last_LED_switch=0;
uint32_t time_since_last_frame = 0;

uint32_t current_milli=0; 

// bool sessionRunning = false;

void setup()
{
  // put your setup code here, to run once:
  pinMode(piezoPin, OUTPUT);
  pinMode(cameraPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  digitalWrite(piezoPin, LOW);
  digitalWrite(cameraPin, LOW);
  //max_frames = trial_duration/frame_period_ms;

  Serial.begin(115200);

  while (Serial.available() == 0) 
  {
  }

}

void loop()
{
  //Trial
    
  start_trial = millis();
  digitalWrite(bluePin,HIGH);
  digitalWrite(greenPin,LOW);     
  total_LED = 1 ;

  digitalWrite(cameraPin, HIGH);
  delay(frame_duration_ms);
  digitalWrite(cameraPin, LOW);
  total_frames = 1;

  while ((millis() - start_trial) < trial_duration)
  {   
    current_milli = millis();
    time_since_last_LED_switch = current_milli - (start_trial + (total_LED-1)*LED_period_ms);

    if (time_since_last_LED_switch > LED_period_ms)
    {
//      Serial.print("time_LED_switch=");
//      Serial.println(millis());
      digitalWrite(bluePin, !digitalRead(bluePin));
      digitalWrite(greenPin, !digitalRead(greenPin));     
      total_LED += 1 ;
     
      current_milli = millis();
      time_since_last_frame = current_milli - (start_trial + (total_frames-1)*frame_period_ms);
      
      if (time_since_last_frame > frame_period_ms)
      {
//        Serial.print("time_frame_switch=");
//        Serial.println(millis());
//        
        digitalWrite(cameraPin, HIGH);
        delay(frame_duration_ms);
        digitalWrite(cameraPin, LOW);
        total_frames += 1;
      }
    }
    
    if ((millis() - start_trial > stim_on) && (millis() - start_trial < stim_off))
    {
      if (millis() - last_piezo_switch > piezo_period_ms)
      {
        last_piezo_switch = millis();          
        digitalWrite(piezoPin,HIGH);
      }  
      if (millis() - last_piezo_switch > piezo_stim_duration_ms)
      {
        digitalWrite(piezoPin,LOW);
      }  
    }
  }  
  

  // Inter trial interval

  digitalWrite(piezoPin,LOW);
  digitalWrite(cameraPin, LOW);
  start_iti = millis();
  total_LED = 0;
  while (millis() - start_iti < 4000)
  {
    if (millis() - (start_iti + total_LED * LED_period_ms) > LED_period_ms)
    {
      digitalWrite(bluePin, !digitalRead(bluePin));
      digitalWrite(greenPin, !digitalRead(greenPin));     
      total_LED += 1 ;
    }
  }  
  Serial.println(total_frames);
  while (millis() - start_iti < iti_duration)
  {   
    if (millis() - (start_iti + total_LED * LED_period_ms) > LED_period_ms)
    {
      digitalWrite(bluePin, !digitalRead(bluePin));
      digitalWrite(greenPin, !digitalRead(greenPin));     
      total_LED += 1 ;
    }
  }    
}
