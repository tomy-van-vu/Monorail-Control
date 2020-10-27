#include <Metro.h>
#include <Servo.h>
#include <Arduino.h>

typedef enum {
  DOOR_CLOSE,
  DOOR_OPEN
} door_position;

typedef struct {
  door_position current_state;
  door_position next_state;
} SM_door;


void MetroDoor(int);
void door_init();
bool open_door(SM_door *);
bool close_door(SM_door *);
