#include "Brakes.h"
#include <Arduino.h>

#define DUTY_CYCLE      66      // percentage
#define ON_DURATION     1500    // ms
#define OFF_DURATION    772     // ms


bool brakes_is_active;
unsigned long time_activated;
unsigned long time_deactivated;
int solenoid_pin;


void brakes_init(int pin){
    solenoid_pin = pin;
    brakes_is_active = false;
    pinMode(pin, OUTPUT);
}


void brakes_on(){
    digitalWrite(solenoid_pin, HIGH);
    time_activated = millis();
    brakes_is_active = true;
}


void brakes_pulse(){
    if (!brakes_is_active) {
        int inactive_duration = millis() - time_deactivated;
        if (inactive_duration >= OFF_DURATION) {
            digitalWrite(solenoid_pin, HIGH);
            time_activated = millis();
            brakes_is_active = true;
        }

    } 
    else if (brakes_is_active) {
        int active_duration = millis() - time_activated;
        if (active_duration >= ON_DURATION) {
            digitalWrite(solenoid_pin, LOW);
            time_deactivated = millis();
            brakes_is_active = false;
        }
    }
}