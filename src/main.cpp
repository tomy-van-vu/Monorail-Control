#include <Arduino.h>

//#include "Brakes.h"
#include "Comms.h"
#include "Colour_sensor.h"
#include "MetroDoor.h"
#include "Motor.h"


#include "BTInterface.h"
// temporary change of serial port for testing on Uno (no second hardware serial port)
//bt_interface bt_i = {4, "INIT", &Serial2};
bt_interface bt_i = {4, "INIT", &Serial};
#include "Helpers.h"


#define BAUD_RATE 9600
#define BLE_PIN 4
#define MOTOR_PIN
#define COLOUR_IN_1
#define COLOUR_IN_2
#define COLOUR_OUT_1
#define DOOR_PIN


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}