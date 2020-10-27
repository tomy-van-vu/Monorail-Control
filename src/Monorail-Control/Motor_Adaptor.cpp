/*
 * Adaptor functions for a state machine to operate a custom motor controller library
 */

#include "Motor_Adaptor.h"
#include "Motor.h"

#define MOTOR_PIN 9


#define NO_HARDWARE // comment out

int slow_speed;
int fast_speed;

// init
Motor my_motor(MOTOR_PIN);

void motor_init() {
  slow_speed = 10;
  fast_speed = 255;
}


bool motor_e_stop(SM_motor *sm) {
  ///// for testing without hardware
  #ifdef NO_HARDWARE
  return true;
  #endif
  
  bool success = false;
  success = my_motor.Stop();
  return success;
}

/*
 * @desc
 * @para
 * @return
 */
bool motor_stop(SM_motor *sm) {
  ///// for testing without hardware
  return true;
  
  bool success = false;
  success = my_motor.Stop();
  return success;
}

/*
 * @desc - Reads given state_machine to determine speed and direction. 
 * Starts the motor and returns whether the motor has successfully reached speed
 * 
 * @para - *sm - contains data describing current and expected next states of the motor
 * @return - bool - whether the motor has reach described state
 */
bool motor_start(SM_motor *sm){
  ///// for testing without hardware
  return true;
  
  bool success = false;
  int start_dir = direction_to_int(sm->current_direction);
  my_motor.Dir_Speed(start_dir, slow_speed);
  success = my_motor.Start();
  return success;
}

/*
 * @desc - Reads given state_machine to determine speed and direction. 
 * Gets the motor to highest speed and returns whether the motor has successfully reached speed
 * 
 * @para - *sm - contains data describing current and expected next states of the motor
 * @return - bool - whether the motor has reach described state
 */
bool motor_fast(SM_motor *sm){
  ///// for testing without hardware
  return true;
  
  bool success = false;
  int start_dir = direction_to_int(sm->current_direction);
  my_motor.Dir_Speed(start_dir, slow_speed);
  success = my_motor.Start();
  return success;
}

/*
 * @desc - Reads given state_machine to determine speed and direction. 
 * Gets the motor to cruising speed and returns whether the motor has successfully reached speed
 * 
 * @para - *sm - contains data describing current and expected next states of the motor
 * @return - bool - whether the motor has reach described state
 */
bool motor_slow(SM_motor *sm){
  ///// for testing without hardware
  return true;
  
  bool success = false;
  int start_dir = direction_to_int(sm->current_direction);
  my_motor.Dir_Speed(start_dir, fast_speed);
  success = my_motor.Start();
  return success;
}


/*
 * Converts enum motor_direction value to int representation
 * 
 * @param motor_direction
 * @return int: -1 or +1
 */
int direction_to_int(motor_direction m_dir) {
  // TODO test and specificy correlation between -1/+1 annd EAST/WEST
  switch (m_dir) {
    case M_EAST:
      return 1;
      break;
    case M_WEST:
      return -1;
      break;
    default:
      return 0;
      break;
  }
  return 0;
}
