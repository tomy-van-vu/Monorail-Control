#include "Colour_sensor.h"
#include <Arduino.h>

// Pins
//#define COLOUROUT A5
//#define COLOURS2 A4
//#define COLOURS3 A3
int  COLOUROUT;
int  COLOURS2;
int  COLOURS3;

// Globals
int frequency = 0;

void colour_sensor_init(int s2, int s3, int out1) {
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out1, INPUT);

  COLOUROUT = out1;
  COLOURS2 = s2;
  COLOURS3 = s3;
}



sensor_colour readColour() {
  int R, G, B;  // Store rgb values from each photodiode filtered readings

  
  // Setting red filtered photodiodes to be read
  digitalWrite(COLOURS2, LOW);
  digitalWrite(COLOURS3, LOW);
  // Reading the output frequency
  R = pulseIn(COLOUROUT, LOW);


  // Setting Green filtered photodiodes to be read
  digitalWrite(COLOURS2, HIGH);
  digitalWrite(COLOURS3, HIGH);
  // Reading the output frequency
  G = pulseIn(COLOUROUT, LOW);


  // Setting Blue filtered photodiodes to be read
  digitalWrite(COLOURS2, LOW);
  digitalWrite(COLOURS3, HIGH);
  // Reading the output frequency
  B = pulseIn(COLOUROUT, LOW);
  
  /*
  Serial.print(R);
  Serial.print("  ");
  Serial.print(G);
  Serial.print("  ");
  Serial.println(B);
  */


   //////////// These are track values
  /*
  //need to play around with values
  if((R>5)  & (R<20) & 
     (G>21) & (G<65) & 
     (B>20) & (B<50)) {
    //Serial.println("Red");
    return CS_RED;
  }
  else 
  if((R>7)  & (R<40) & 
     (G>11) & (G<20) & 
     (B>7)  & (B<14)){
    //Serial.println("Blue");
    return CS_BLUE;
  }
  else 
  if((R>20) & (R<33) & 
     (G>11) & (G<24) & 
     (B>28) & (B<45)){
    //Serial.println("Green");
    return CS_GREEN;
  }
  else 
  if((R>2) & (R<8) & 
     (G>5) & (G<10) & 
     (B>9) & (B<16)){
    //Serial.println("Yellow");
    return CS_YELLOW;
  }
  */


 int vrange = 9;

 bool print_c = true;

  
  //////////// These are test values
  
  //need to play around with values
  if((R>14-vrange) & (R<14+vrange) & 
     (G>47-vrange) & (G<47+vrange) & 
     (B>37-vrange) & (B<37+vrange)) {
    if(print_c) {Serial.println("Red");}
    return CS_RED;
  }
  else 
  if((R>23-vrange) & (R<23+vrange) & 
     (G>16-vrange) & (G<16+vrange) & 
     (B>26-vrange) & (B<26+vrange)){
    if(print_c) {Serial.println("Green");}
    return CS_GREEN;
  }
  else 
  if((R>8-vrange)  & (R<8+vrange) & 
     (G>10-vrange)  & (G<10+vrange) & 
     (B>17-vrange) & (B<17+vrange)){
    if(print_c) {Serial.println("Yellow");}
    return CS_YELLOW;
  }
  else 
  if((R>37-vrange)  & (R<37+vrange) & 
     (G>19-vrange)  & (G<19+vrange) & 
     (B>11-vrange)  & (B<11+vrange)){
    if(print_c) {Serial.println("Blue");}
    return CS_BLUE;
  }
  if(print_c) {Serial.println("None");}
  return CS_NONE;
  
}



// check if there is a change in colour reading from the sensor
sensor_colour check_colour_sensor(SM_colour_sensor *colour_sensor) {

  int store[] = {0, 0, 0, 0, 0};
  int poll_count;
  sensor_colour colour_in;

  for (poll_count = 5; poll_count > 0; poll_count = poll_count - 1) {
    colour_in = readColour();
    store[colour_in] = store[colour_in]  + 1;
  }

  colour_in = CS_NONE;
  int c;
  for (c=1; c<5; c=c+1){
    if (store[c] > store[colour_in]) {
      colour_in = (sensor_colour)c;
    }
  }
  
  
  
  // no colour change
  if (colour_sensor->current_colour == colour_in) {
    colour_sensor->colour_updated = false;
    return colour_in;
  } else {
    // 
    colour_sensor->colour_updated = true;
    colour_sensor->last_colour = colour_sensor->current_colour;
    colour_sensor->current_colour = colour_in;    
    return colour_in;
  }
  return CS_NONE;
}
