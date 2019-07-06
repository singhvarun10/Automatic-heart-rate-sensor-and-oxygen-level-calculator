#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <rgb_lcd.h>
#include <Stepper.h>

#define REPORTING_PERIOD_MS     1000
#define stepsPerRevolution  200
#define key 8                                          // key digital pin
#define buz 4                                          // buzzer digital pin
#define LHB 30                                         // critical lowest heart rate
#define HHB 100                                         // critical highest heart rate

Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);
rgb_lcd lcd;
PulseOximeter pox;
uint32_t tsLastReport = 0;

///////////////////////////////////////////////////////

void setup() {
  pinMode(key,INPUT);
  pinMode(buz,OUTPUT);
  myStepper.setSpeed(100);
  lcd.begin(16,2);
  pox.begin();
}

///////////////////////////////////////////////////////

void loop() {
  pox.update();                                                  // geting new data from heart rate sensor
  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    LCDwrite();                                                    // showing data on LCD
    LCDcolor();                                                    // choosing LCD's backlight color
    inject();                                                      // inject drug if situation is dangerous
    tsLastReport = millis();
  }
  buzer();                                                         // sound the alarm if the button is pressed or situation is dangerous
}

///////////////////////////////////////////////////////

void LCDwrite(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HEART:: ");
  lcd.print(pox.getHeartRate());
  lcd.setCursor(0,1);
  lcd.print("SPO2:: ");
  lcd.print(pox.getSpO2());
}

void LCDcolor(){
  if (LHB<pox.getHeartRate() and pox.getHeartRate()<HHB)        // dangerous situation
    {
      lcd.setRGB(0, 0, 255);                           // blue backlight
    }
    
  else
    {
      lcd.setRGB(255, 0, 0);                         // red backlight
    }
}

void buzer(){
  // dangerous situation or pressed button
  if ( digitalRead(key)==HIGH or pox.getHeartRate()<LHB or pox.getHeartRate()>HHB)
    {
      digitalWrite(buz,HIGH);
    }
  else
    {
      digitalWrite(buz,LOW);
    }
}

void inject(){
  if (pox.getHeartRate()>HHB)                          // dangerous high heart rate
    {
      myStepper.step(2075);                              // spin 1 round and inject drug
    }
}
