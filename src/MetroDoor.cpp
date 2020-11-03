#include "MetroDoor.h"

#include <Metro.h>
#include <Servo.h>
#include <Arduino.h>


Servo my_door;
bool closed;
bool opened;
int state;
int hook;
unsigned long previousMillis;

void MetroDoor_init(int pin)
{
    my_door.attach(pin);
    closed = true;
    opened = false;
    state = 0;
    hook = 0;

    close_door();
}

bool open_door()
{
    if (opened == true)
    {
        return true;
    }
    compute(1);
    return false;
}

bool close_door()
{
    if (closed == true)
    {
        return true;
    }
    compute(-1);
    return false;
}

void compute(int val) // 1 = open; -1 = close;
{
    unsigned long currentMillis = millis();

    if (val == 1 && state == 0)
    {
        previousMillis = currentMillis;
        state = 1;
        hook = val;
        my_door.writeMicroseconds(1600);    
    }
    else if (val == -1 && state == 0)
    {
        previousMillis = currentMillis;
        state = 1;
        hook = val;
        my_door.writeMicroseconds(1400);
    }

    if (currentMillis - previousMillis >= 3000)
    {
        state = 0;
        my_door.writeMicroseconds(1500);
        if (hook == 1)
        {
            opened = true;
            closed = false;
        }
        else if (hook == -1)
        {
            closed = true;
            opened = false;
        }
        return;
    }
}
