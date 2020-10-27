#include "COMMS.h"
#include <Arduino.h>

// function to read from bluetooth
// Written by T4C2
message read_msg(){
  message blue_in = NONE;
  if (Serial2.available() > 0) blue_in = (message)Serial2.read();
  //if (Serial.available()) {blue_in = (message)Serial.read();}
  return blue_in;
}
