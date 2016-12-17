/*    Sensors Class
 *   
 *    Sensor Manager class to clean up the main file
 *  
 */

#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"

class Sensors
{
    public:
        Sensors(int pin);
        void check();
        int value;
    private:
        int _pin;
        long debounce;
        int last_status;
        int debounce_delay;
};



#endif

