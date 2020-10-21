#include "Arduino.h"


// Pins
#define COLOUROUT A5
#define COLOURS2 A4
#define COLOURS3 A3

// Globals
int frequency = 0;
int color;


// Constructor
void ColourSensor() {	
  pinMode(COLOUROUT, INPUT);
  pinMode(COLOURS2, OUTPUT);
  pinMode(COLOURS3, OUTPUT);
  Serial.begin(9600);

}


int readColour() {
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
    color = 1; // Red
  }
  if (G<56 & G>9 & B<58 & B>17) {
    color = 2; // Green
  }
  if (R<95 & R>35 & G == 35 & B<75 & B>22) {
    color = 3; // Blue
  }
  if (R<11 & R>4 & G<12 & G>6 & B<20 & B>12) {
    color = 4; // Yellow
  }
  color = 0;

  return color;

}



