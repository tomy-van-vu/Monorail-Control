#include "Door.h"

#define DOOR_PIN 12
#define METRO_MILLIS 5000

Servo door;
Metro time = Metro(METRO_MILLIS);

bool door_state_closed;
bool door_state_opened;
int last_closed_state;
int last_opened_state;


void door_init() {
  door.attach(DOOR_PIN);
  door_state_closed = false;
  door_state_opened = false;
}

bool close_door(SM_door *door_state){
  if (door_state_closed == true){
    door.writeMicroseconds(1500);
    return true;
  }

  if (last_closed_state == 0){
    time.reset();
    last_closed_state = 1;
    door.writeMicroseconds(2000);
  }

  if (time.check() == 1){
    door_state_closed = true;
  }
  return false;
}


bool open_door(SM_door *door_state){
  if (door_state_opened == true){
    door.writeMicroseconds(1500);
    return true;
  }

  if (last_opened_state == 0){
    time.reset();
    last_opened_state = 1;
    door.writeMicroseconds(1000);
  }

  if (time.check() == 1){
    door_state_opened = true;
  }
  return false;
}
