/*   Andreja Premium Control Board Replacement 
 *   Written By Anthony DiPilato
 *   
 *   This project replaces the Gicar control board from a Quickmill Andreja Premium with
 *   an Arduino Uno, a four channel relay board (3 channels used), and a piezo buzzer.
 *   $20 in parts to replace a ~$200 control board.
 *   
 *   Additional details can be found at [url]
 *   
 *   The arduino monitors the two float switches in the water reservior and the water level probe in the boiler.
 *   The boiler temperature is maintained by the pressurestat.
 *   
 *   Upper float switch triggers audible alarm.
 *   Lower switch cuts power to the boiler to protect the heating element from overheating.
 *   Water probe triggers the water pump and actuated valve to fill the boiler.
 *   
 *   USE AT YOUR ON RISK.
 *   I am posting this project for EDUCATIONAL USE ONLY.
 *   This project involves electricity, high heat, and water/steam.
 *   I will not be held liable damages and/or injuries resulting from the use of this code or from copying this project.
 *   
 */

// Configuration
#include "Configuration.h"

// Include Melody library and Sensors class
#include "Melody.h"
#include "Sensors.h"



// Setup melodies
Melody sw_melody(5); // include pin
Melody alert(5); // include pin
// Setup sensor pins
Sensors boiler(9);
Sensors upper_water(3);
Sensors lower_water(2);

// Status for heater valve and pump
int heat_state = 1;
int pump_state = 0; // pump and valve are shared

long pump_time = 0;

// We are going to do a 5% chance to play star wars theme when it starts up
int sw = random(20);

void setup() {
    Serial.begin(9600);
 
    // Star Wars Imperial March
    // Because who doesn't want to listen to that first thing in the morning
    int g4 = 392; int ds4 = 311; int as4 = 466; int d5 = 587; int ds5 = 622; int fs4 = 370;
    int sw_notes[] = {0,g4,g4,g4,ds4,as4,g4,ds4,as4,g4,d5,d5,d5,ds5,as4,fs4,ds4,as4,g4};
    int sw_beats[] = {1,3,3,3,2,1,3,2,1,4,3,3,3,2,1,3,2,1,4};
    sw_melody.create(sw_notes, sw_beats, 19, 0, 600, 1000);
    sw_melody.start();

    // Alert Tone for low water
    int a = 880; int R = 0;
    int alert_notes[] = {R,a,a,a,R,R,R,R};
    int alert_beats[] = {2,1,1,1,4,4,4,4};
    alert.create(alert_notes, alert_beats, 8, 1, 1000, 1000);
  
    // Output mode for relays
    pinMode(PUMP, OUTPUT);       
    pinMode(VALVE, OUTPUT);
    pinMode(HEAT, OUTPUT);

    // Initialize relays
    // Pump and value off, Heat on
    // Heat is on N/O relay
    digitalWrite(PUMP, HIGH);
    digitalWrite(VALVE, HIGH);  
    digitalWrite(HEAT, HIGH);
  
}

// Serial Output for debugging
void debug_output(){
    Serial.print("Boiler: ");
    Serial.println(boiler.value);
    Serial.print("Upper Water: ");
    Serial.println(upper_water.value);
    Serial.print("Lower Water: ");
    Serial.println(lower_water.value);
    delay(250);
}

void toggle_heat(int state){
    if(state == heat_state){
        return;
    }
    switch(state){
        case 0:
            digitalWrite(HEAT, LOW); // LOW is on, N/O relay
            break;
        case 1:
            digitalWrite(HEAT, HIGH);
            break;
    }
    heat_state = state;
    return;
}

void toggle_pump(int state){
  if(state == pump_state){
    return;
  }
  if(state == 0){
      if ((millis() - pump_time) > PUMP_DELAY) {
          // Turn off pump before valve
          digitalWrite(PUMP, HIGH); // High is off
          digitalWrite(VALVE, HIGH);
          pump_state = state;
      }
  }else{
      // Turn on valve before pump
      digitalWrite(VALVE, LOW);
      digitalWrite(PUMP, LOW);
      pump_state = state;
  }
  return;
}

void check_sensors(){
    // Read Sensors
    boiler.check();
    upper_water.check();
    lower_water.check();
    // If upper water sensor 0 trigger alert
    if(upper_water.value == 0){
        // start alert melody
        if(alert.melody_status == 0){
            alert.start();
        }
        alert.loop_();
    }else{
        // Make sure alert is not playing
        if(alert.melody_status == 1){
            alert.stop_();
        }
    }// end upper check
    // If lower water sensor 0 shut off power to heating element
    if(lower_water.value == 0){
        toggle_heat(0);
        // also turn off pump so it doesn't run dry
        toggle_pump(0);
    }else{
        toggle_heat(1);
    }
    // If boiler water is low turn on pump
    if(boiler.value == 0 && lower_water.value != 0){
        toggle_pump(1);
    }else{
        toggle_pump(0);
    }
}


// Main Loop
void loop() {
    if(sw == 0 ){
        sw_melody.loop_();
    }
    // read sensors
    check_sensors();  
    // Debug Output
    if(DEBUG == true){ debug_output();}
}
