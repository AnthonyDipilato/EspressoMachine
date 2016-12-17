/*   Arduino Simple Melody Library 
 *   Written By Anthony DiPilato
 *   
 *  A simple melody library for a piezo buzzer.
 *  
 */

#include "Arduino.h"
#include "Melody.h"



// Initiate pin
Melody::Melody(int pin){
    pinMode(pin, OUTPUT);
    _pin = pin;
}

// Add notes for melody
void Melody::create(int notes_array[], int beats_array[], int numNotes_, int repeat_, int tempo_, int pause_){
    notes = notes_array;
    beats = beats_array;
    numNotes = numNotes_;
    repeat = repeat_;
    pause = pause_;
    tempo = tempo_;
    // melody_status
    pos = 0;
    melody_status = 0;
    playing = 0;
    next = 0;
}


// start melody from beginning
void Melody::start(){
    melody_status = 1;
    pos = 0;
}

void Melody::stop_(){
    melody_status = 0;
    noTone(_pin);
}

void Melody::play_tone(int note_tone){
    noTone(_pin);
    delay(50);
    tone(_pin, note_tone);
}

void Melody::loop_(){
    // If melody is active and ready for next note
    if(melody_status == 1 && millis() > next){
        pos++;
        // if at end of melody 
        if(pos == numNotes){
            if(repeat == 1){
              pos = 0;
            }else{
              melody_status = 0;
              noTone(_pin);
              return;
            }
        }
        // Current note
        note_tone = notes[pos];
        if(note_tone == 0){
            noTone(_pin);
        }else{
            play_tone(note_tone);
            Serial.print("note :");
            Serial.println(note_tone);
        }
        // Next Note
       next = ((tempo / 4) * beats[pos]) + millis();
    }
}


 
