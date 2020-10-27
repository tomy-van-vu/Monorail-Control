#include "CS.h"
#include <Arduino.h>

// Pins
#define COLOUROUT A5
#define COLOURS2 A4
#define COLOURS3 A3

// diode colour detection ranges

sensor_range rgb_red =    {6, 29,   6, 18,    0, 0};
sensor_range rgb_green =  {0, 0,    9, 56,    17, 58};
sensor_range rgb_blue =   {35, 95,  35, 35,   22, 75};
sensor_range rgb_yellow = {4, 11,   6, 12,    12, 20};

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


  //still need to play around with values
  if (R<19 & R>6 & G<18 & G>6) {
    return CS_RED;
  }
  if (G<56 & G>9 & B<58 & B>17) {
    return CS_GREEN;
  }
  if (R<95 & R>35 & G == 35 & B<75 & B>22) {
    return CS_BLUE;
  }
  if (R<11 & R>4 & G<12 & G>6 & B<20 & B>12) {
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
