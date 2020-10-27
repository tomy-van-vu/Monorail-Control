

typedef enum {
  M_STOP, 
  M_START, 
} motor_state;

typedef enum {
  M_IDLE, // stationary
  M_SLOW,
  M_FAST,
} motor_speed;

typedef enum { 
  M_NONE,
  M_EAST, 
  M_WEST,
} motor_direction;

typedef struct {
  motor_state current_state;
  motor_state next_state;
  motor_speed current_speed;
  motor_speed next_speed;
  motor_speed target_speed; // specifically for when going fast in one direction, then needing to change to fast in opposite direction
  motor_direction current_direction;
  motor_direction next_direction;
  int pwm;
} SM_motor;


bool motor_stop(SM_motor *);
bool motor_start(SM_motor *);
bool motor_fast(SM_motor *);
bool motor_slow(SM_motor *);
