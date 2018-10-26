/*
 Grove Temperature & Humidity Sensor Pro
 
 Read value from Temperature & Humidity sensor then display on 
 the Grove 4-Digit Display.
 
 The circuit:
 * 4-Digit Display attached to Pin 38 and 39 (J14 plug on Grove Base BoosterPack)
 * Rotary Angle Sensor attached to Pin 24 (J6 plug on Grove Base BoosterPack)
 * sig pin of the Grove-Temperature-Humidity Sensor to the analog pin A1

 
 * Note: Put your hands on Temperature & Humidity Sensor, both of the values 
         will rise. 
         
         4-Digit Display:
         |--------------------------|
         | temperature :  humidity  |    
         |--------------------------| 
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-TemperatureHumidity-Sensor-Pro-p-838.html 
 
 */
 
#include "TM1637.h" 
#include "DHT.h"

/* Macro Define */
#define BUZZER_PIN               39            /* sig pin of the Grove Buzzer */
#define BLINK_LED         RED_LED            /* blink LED */
#define TEMP_HUMI_PIN     24                 /* pin of temperature&humidity sensor */


/* Global Variables */
DHT dht(TEMP_HUMI_PIN, DHT22);            /* temperature&humidity sensor object */
String alarmStatus = "OFF";


/* the setup() method runs once, when the sketch starts */
void setup() 
{    
    dht.begin();                         /* initialize temperature humidity sensor */
        
    pinMode(BLINK_LED, OUTPUT);            /* declare the LED pin as an OUTPUT */
    pinMode(BUZZER_PIN, OUTPUT);          /* declare the BUZZER pin as an OUTPUT */
   
   
    Serial.begin(2000000);
}

/* the loop() method runs over and over again */
void loop() 
{   
    int temp_cel = dht.readTemperature();             /* read the temperature value from the sensor */
    int temp_feh = (temp_cel * 9/5) + 32;               /* calculate fahrenheit */
    int humidity = dht.readHumidity();                   /* read the humidity value from the sensor */    

    
    if(temp_feh >= 75 && alarmStatus == "OFF"){
      //Serial.print("Temperature is: ");
      //Serial.println(temp_feh);
      //Serial.print("F");
      turnOnAlarm();
      alarmStatus = "ON";
    }else if(temp_feh < 75 && alarmStatus == "ON"){
      //Serial.print("Temperature is: ");
      //Serial.println(temp_feh);
      //Serial.print("F");
      turnOffAlarm();
      alarmStatus = "OFF";
    }
        Serial.print("Temperature:");
        //Serial.print(temp_cel);
        //Serial.print("C");
        //Serial.print(" / ");
        Serial.print(temp_feh);
        Serial.print(",");
        //Serial.print("F");
    
        
        Serial.print("Humidity:");
        Serial.print(humidity);
        Serial.println();
  
    //delay(300);
    

}

void turnOnAlarm(){
  //Serial.println("!!!!!!!!RED ALERT TEMPERATURE IS TOO HIGH!!!!!!!!!");
  digitalWrite(BUZZER_PIN, HIGH);
}

void turnOffAlarm(){
  //Serial.println("TEMPERATURE HAS LOWERED TO NORMAL LEVELS");
  digitalWrite(BUZZER_PIN, LOW);
}
