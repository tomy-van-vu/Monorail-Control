#include "COMMS.h"
#include <Arduino.h>


// function to read from bluetooth
// Written by T4C2
message read_msg(){
  message blue_in = NONE;
  if (Serial3.available() > 0) blue_in = (message)Serial3.read();
  return blue_in;
}

message testing_mode_read_message(){
  message blue_in = NONE;
  if (Serial.available()) {blue_in = (message)Serial.read();}
  return blue_in;
}
