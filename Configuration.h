/*    Configuration
 *   
 *    Default configurations for Espresso Machine
 *  
 */

#ifndef Configuration_h
#define Configuration_h

 // Debug Settings
 #define DEBUG true
 #define DEBUG_DELAY 1000

// Setup Relay Pins
#define VALVE 8 // Was pin 13 but is connected to led which blinks on startup triggering the relay
#define PUMP 12
#define HEAT 11

// Pump Delay
#define PUMP_DELAY 500


#endif


