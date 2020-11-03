#include <Arduino.h>
#include <Encoder.h>
#include <PID_v1.h>
#include <Servo.h>


#include "Motor.h"

//Encoder myEnc(22, 23);
double Setpoint, Input, Output;
double consKp = 0.1, consKi = 0.0001, consKd = 0.2;

unsigned long preStop; // millies timer
unsigned long pSlow;   // millies timer
unsigned int motorOut; // holds the value to be writen to the motor.
unsigned int speed;        // holds the set speed
unsigned int speed_hold; 
int dir;               // hold the dirrection of the train
long oldPosition;      // holds the encorder data
long distance;         // The distance need to be traveled by PID.

Servo my_motor;


void Motor_init(unsigned int pin)
{
  my_motor.attach(pin);
  preStop = 0;
  motorOut = 1500;
}

void motorwrite()
{

  if (motorOut >= 2000)
  {
    motorOut = 2000;
  }
  else if (motorOut <= 1000)
  {
    motorOut = 1000;
  }
  my_motor.write(motorOut);
}

bool stop()
{
  if (motorOut != 1500)
  {
    unsigned long stopMillis = millis();

    if (stopMillis - preStop >= 10) // check if the metro has passed its interval .
    {
      preStop = stopMillis;
      if (motorOut > 1500)
      {
        motorOut--;
      }
      else if (motorOut < 1500)
      {
        motorOut++;
      }
      motorwrite();
    }
  }
  return true;
}

bool motor_start()
{
  if (motorOut != speed)
  {
    unsigned long slow = millis();

    if (slow - pSlow >= 10)
    {
      pSlow = slow;

      if (dir == 1 && motorOut != speed)
      {
        if (motorOut > speed)
        {
          motorOut--;
        }
        else if (motorOut < speed)
        {
          motorOut++;
        }
      }
      else if (dir == -1 && motorOut != speed)
      {
        if (motorOut > speed)
        {
          motorOut--;
        }
        else if (motorOut < speed)
        {
          motorOut++;
        }
      }
      motorwrite();
    }
    return false;
  }
  return true;
}

void Dir_Speed(int dir_val, unsigned int speed_val)
{
  if (dir_val == dir && speed_val == speed_hold)  // if the value already exist return
  {
    return;
  }
  speed_hold = speed_val;

  if (dir_val == -1)
  {
    dir = -1;
    speed = map(speed_hold, 0, 255, 1500, 1000);
  }
  else if (dir_val == 1)
  {
    dir = 1;
    speed = map(speed_hold, 0, 255, 1500, 2000);
  }
}


bool motor_fast(SM_motor *sm) {
  motor_direction d = sm->current_direction;
  int d2 = get_direction(d);

  Dir_Speed(d2, 255);
  bool s = motor_start();
  return s;
}


bool motor_stop(SM_motor *sm) {
  motor_direction d = sm->current_direction;
  int d2 = get_direction(d);

  Dir_Speed(d2, 0);
  bool s = motor_start();
  return s;
}



bool motor_slow(SM_motor *sm) {
  motor_direction d = sm->current_direction;
  int d2 = get_direction(d);

  Dir_Speed(d2, 100);
  bool s = motor_start();
  return s;
}

int get_direction(motor_direction md) {
  if (md == M_EAST) {
    return -1;
  } else {
    return 1;
  }
  
  return 0;
}

bool motor_brake(SM_motor *sm)
{
  my_motor.write(1500);
  return true;
}

/*
void Motor::SetDistance(long var)
{

  if (var != Setpoint)
  { // checks if we received new value and updates setpoint.
    Setpoint = var;
  }
  int out;
  double gap = (Setpoint - Input);
  if (gap < 0)
  {
    myPID.SetControllerDirection(REVERSE);
    out = map(Output, 0, 500, 1500, 2000);
  }
  else
  {
    myPID.SetControllerDirection(DIRECT);
    out = map(Output, 0, 500, 1500, 1000);
  }
  myPID.Compute();
  value.motorOut = out;
  motorwrite();
}
*/
