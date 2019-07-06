#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Stepper.h> 
#define STEPS 32
#define REPORTING_PERIOD_MS     1000
#include<SoftwareSerial.h>
PulseOximeter pox;

uint32_t tsLastReport = 0;
Stepper stepper(STEPS, 8, 10, 9, 11);
/* Create object named bt of the class SoftwareSerial */ 
SoftwareSerial bt(7 ,5); /* (Rx,Tx) */ 

int val = 0;
double k =0;
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);
    stepper.setSpeed(1000);
    Serial.println("Initializing MAX30100");
    pox.begin();
    pox.setOnBeatDetectedCallback(onBeatDetected);
    bt.begin(9600); /* Define baud rate for software serial communication */
    Serial.begin(9600); /* Define baud rate for serial communication */
}


 


void loop()
{
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / k value:");
        k=pox.getHeartRate()*2;
        Serial.print(k);
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.print("% / temp:");
       Serial.print(pox.getTemperature());
        Serial.println("C");

        tsLastReport = millis();
        bt.print(k);
        bt.print(",");
        bt.print(pox.getSpO2());
        bt.print(",");
        bt.print(pox.getTemperature());
       
    }
    if(k>50)
    {
      val=2048;
      stepper.step(val);
      Serial.println(val);
      delay(1000);
    }
}
