typedef enum {
  M_STOP, 
  M_START, 
} motor_state;

typedef enum {
  M_IDLE, // stationary
  M_SLOW, // pre-determined speed
  M_FAST, // pre-determined speed
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
  motor_speed target_speed;
  motor_direction current_direction;
  motor_direction next_direction;
  int pwm;
} SM_motor;



//PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void Motor_init(unsigned int); // constructor
bool stop();  // returns true when the train has stopped.
bool motor_start(); // returns true when the train has reached set speed.
void Dir_Speed(int, unsigned int); // set the dirrection(1 or -1) and speed(0-255)
void SetDistance(long);

bool motor_fast(SM_motor *);
bool motor_slow(SM_motor *);
bool motor_stop(SM_motor *);

void motorwrite();     // function to write to the motor.

int get_direction(motor_direction);
