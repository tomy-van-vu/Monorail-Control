#include "Colour_sensor.h"
#include <Arduino.h>

// Pins
#define COLOUROUT A5
#define COLOURS2 A4
#define COLOURS3 A3

// Globals
int frequency = 0;

void colour_sensor_init() {
  pinMode(COLOUROUT, INPUT);
  pinMode(COLOURS2, OUTPUT);
  pinMode(COLOURS3, OUTPUT);
}



sensor_colour readColour() {
  // Setting red filtered photodiodes to be read
  digitalWrite(COLOURS2, LOW);
  digitalWrite(COLOURS3, LOW);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int R = frequency;


  // Setting Green filtered photodiodes to be read
  digitalWrite(COLOURS2, HIGH);
  digitalWrite(COLOURS3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int G = frequency;


  // Setting Blue filtered photodiodes to be read
  digitalWrite(COLOURS2, LOW);
  digitalWrite(COLOURS3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int B = frequency;

  //need to play around with values
  if(R>5 & R<20 & G>21 & G< 65 & B>20 & B<50) {
    return CS_RED;
  }
  else if(R>7 & R<40 & G>11 & G<20 & B>7 & B<14){
    return CS_BLUE;
  }
  else if(R>20 & R<33 & G>11 & G<24 & B>28 & B<45){
    return CS_GREEN;
  }
  else if(R>2 & R<8 & G>5 & G<10 & B>9 & B<16){
    return CS_YELLOW;
  }

  return CS_NONE;

}



// check if there is a change in colour reading from the sensor
bool check_colour_sensor(SM_colour_sensor *colour_sensor) {
  sensor_colour colour_in = readColour();

  // no colour change
  if (colour_sensor->current_colour == colour_in) {
    colour_sensor->colour_updated = false;
    return false;
  } else {
    // 
    colour_sensor->colour_updated = true;
    colour_sensor->last_colour = colour_sensor->current_colour;
    colour_sensor->last_colour = colour_in;
    return true;
  }
  return false;
}
