/*    Sensors Class
 *   
 *    Sensor Manager class to clean up the main file
 *  
 */

#include "Arduino.h"
#include "Sensors.h"

// Initiate pin for sensor
Sensors::Sensors(int pin)
{
    // INPUT_PULLUP allows us to wire them to ground using the chip's onboard pullup resistor.
    pinMode(pin, INPUT_PULLUP);
    _pin = pin;
    value = 1;
    last_status = 1;
    debounce_delay = 250; 
}

void Sensors::check(){
    int sensor_value;
    // read sensor
    int reading = digitalRead(_pin);
    // INPUT_PULLUP inverses reading
    if(reading == HIGH){
        sensor_value = 0;
    }else{
        sensor_value = 1;
    }
    // if value does not match last state reset debounce timer
    if(sensor_value != last_status){ 
        debounce = millis(); 
   }
   // if reading is stable past debounce timer we can set it
   if ((millis() - debounce) > debounce_delay) {
      if(sensor_value != value){
          value = sensor_value;
      }
   }
    // Save sensor for last state debounce
    last_status = sensor_value;
}




 
