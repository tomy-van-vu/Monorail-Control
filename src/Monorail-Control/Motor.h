#include <Arduino.h>
#include <Encoder.h>
#include <PID_v1.h>
#include <Servo.h>

class Motor
{
    Servo motor;
    //Encoder myEnc(22, 23);
    double Setpoint, Input, Output;
    double consKp = 0.1, consKi = 0.0001, consKd = 0.2;

    //PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

public:
    Motor(unsigned int); // constructor
    bool Stop();  // returns true when the train has stopped.
    bool Start(); // returns true when the train has reached set speed.
    void Dir_Speed(int, unsigned int); // set the dirrection(1 or -1) and speed(0-255)
    void SetDistance(long);

private:
    void motorwrite();     // function to write to the motor.
    unsigned long preStop; // millies timer
    unsigned long pSlow;   // millies timer
    unsigned int motorOut; // holds the value to be writen to the motor.
    unsigned int speed;        // holds the set speed
    unsigned int speed_hold; 
    int dir;               // hold the dirrection of the train
    long oldPosition;      // holds the encorder data
    long distance;         // The distance need to be traveled by PID.
};