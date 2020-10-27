#include "MetroDoor_Adaptor.h"
#include "MetroDoor.h"


#define DOOR_PIN 12

MetroDoor my_door(DOOR_PIN);

void door_init() {
}

bool close_door(SM_door *door_state){
  ///// for testing without hardware
  return true;
  
  bool success = false;
  success = my_door.close();
  return success;
}


bool open_door(SM_door *door_state){
  ///// for testing without hardware
  return true;
  
  bool success = false;
  success = my_door.open();
  return success;
}
