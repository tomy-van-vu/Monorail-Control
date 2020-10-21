#include <Arduino.h>
#include <Encoder.h>
#include <Metro.h>
#include <PID_v1.h>
#include <Servo.h>

double Setpoint, Input, Output;

double consKp = 0.1, consKi = 0.0001, consKd = 0.2;

PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

Servo motor;

Servo door;

Encoder myEnc(22, 23);

long oldPosition = -999;

int motorOut = 1500; // value to be writen out to the motor

int stopState = HIGH;

Metro metroStop = Metro(0);
Metro doorTime = Metro(0);

boolean doorState = false;

long disthold;

 enum {
  STOP,
  START,
  EAST,
  WEST,
  OPEN,
  CLOSE,
  EMERGENCY
} metrostate;

void Stop()
{

  if (motorOut != 1500)
  {
    int val;

    if (motorOut > 1500)
    {
      val = -1;
    }
    else if (motorOut < 1500)
    {
      val = 1;
    }
    else
    {
      val = 0;
    }

    if (metroStop.check() == 1) // check if the metro has passed its interval .
    {
      if (stopState == HIGH)
      {
        stopState = LOW;
        motorOut = +val;
      }
      else
      {
        stopState = HIGH;
      }
      motor.write(motorOut);
    }
  }
}

void setDistance(long var)
{
  if (disthold != var)
  { // checks if we received new value and updates setpoint.
    Setpoint = var;
    disthold = var;
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
  motor.write(out);
}


void metroDoor(int pos){

  if(pos == 1 && doorState == false){ // close door
    doorState = true;

  }
  else if(pos == 0 and doorState == true){ // open door
    doorState = false;




  }







}







void setup()
{
  motor.attach(13);
  door.attach(12);
  myPID.SetOutputLimits(0, 500);
  myPID.SetMode(AUTOMATIC);
  Serial.begin(115200);
  
}

void loop()
{
  
  switch (metrostate)
  {
  case STOP:
    Stop();
    Serial.println("STOP");
    break;
  case START:
  Serial.println("START");
    // Start();
    break;
  case EAST:
    // do something
    Serial.println("EAST");
    break;
  case WEST:
    //do something
    Serial.println("WEST");
    break;
  case OPEN:
    metroDoor(1); // '1' to open door and '0' to close
    Serial.println("OPEN");
    break;
  case CLOSE:
    metroDoor(0); // '1' to open door and '0' to close
    Serial.println("CLOSE");
    break;
  case EMERGENCY:
    // do something
    Serial.println("EMERGENCY");
    break;
  }

  long newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    oldPosition = newPosition;
    Input = newPosition;
  }
 
}

