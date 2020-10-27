#include "timer_func.h"
#include <Arduino.h>

unsigned long start_time;
unsigned long end_time;
unsigned long time_taken;


void s() {
  start_time = millis();
}
void e(){
  end_time = millis();
  time_taken = end_time - start_time;
  Serial.print("Time taken:  ");
  Serial.println(time_taken);
}
