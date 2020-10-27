#include "Motor.h"

Motor ::Motor(unsigned int pin)
{
  motor.attach(pin);
  preStop = 0;
  motorOut = 1500;
}

void Motor::motorwrite()
{

  if (motorOut >= 2000)
  {
    motorOut = 2000;
  }
  else if (motorOut <= 1000)
  {
    motorOut = 1000;
  }
  motor.write(motorOut);
}

bool Motor ::Stop()
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

bool Motor::Start()
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

void Motor::Dir_Speed(int dir_val, unsigned int speed_val)
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