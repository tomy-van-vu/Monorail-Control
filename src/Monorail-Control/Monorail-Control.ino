#include "Brakes.h"
#include "Comms.h"
#include "Colour_sensor.h"
#include "MetroDoor_Adaptor.h"
#include "Motor_Adaptor.h"


#include "BTInterface.h"
bt_interface bt_i = {4, "INIT", &Serial2};
#include "Helpers.h"


#define BAUD_RATE 9600


typedef struct {
  SM_colour_sensor colour_sensor;
  SM_motor motor;
  SM_door train_door;
  SM_instructions instruction;
  message last_tx;
} SM_train;


// SM declarations
SM_colour_sensor colour_sensor = {CS_NONE, CS_NONE, false};
SM_motor motor = {M_STOP, M_STOP, M_IDLE, M_IDLE, M_IDLE, M_EAST, M_EAST, 0};
SM_door train_door = {DOOR_CLOSE, DOOR_CLOSE};
SM_instructions instructions = {O_NONE, O_NONE, false};
message tx = {NONE};
SM_train train_state = {colour_sensor, motor, train_door, instructions, tx};


// test /////////////////////////////////////////////////////////////////// test
bool debug_mode = true;
unsigned long print_timer = millis();
int print_period = 4000;
int p_counter = 0;






/********************************************************************************/
/********************************************************************************/
/********************************    SETUP    ***********************************/
/********************************************************************************/
/********************************************************************************/
void setup() {
  Serial.begin(BAUD_RATE);
  //Serial2.begin(BAUD_RATE);
  pinMode(4, INPUT);

  door_init();
  colour_sensor_init();
}


/********************************************************************************/
/********************************************************************************/
/********************************    LOOP    ************************************/
/********************************************************************************/
/********************************************************************************/

void loop() {
  // check BLE
  message bt_inc = read_msg();
  if (bt_inc) {
    update_operator_instruction(bt_inc);
  }
  
  // colour sensor
  // commented out - function takes ~2100ms when no colours are detected
  //bool colour_change = check_colour_sensor(&colour_sensor);
  //if (colour_change) {
  if(train_state.colour_sensor.colour_updated) {
    //update_next_state_CS();
  }

  // update state machine
  ready_next_state();
  do_state_transition();


  // DEBUG
  if (debug_mode) {
    if (millis() - print_timer >= print_period) {
      print_timer = millis();


      print_counter();

      print_instruction();
      print_colour();
      print_motor();
      print_speed();
      print_direction();
      print_door();

      Serial.println();

    }
  }

}


/********************************************************************************/
/********************************************************************************/
/*************    STATE UPDATERS - CONTROL BOX INSTRUCTION    *******************/
/********************************************************************************/
/********************************************************************************/

void update_operator_instruction(message new_instruction) {
  switch (new_instruction) {

    // cases 48-55, temporary value for inputs via serial to emulate incoming BLE comms
    // for testing without BLE

    case NONE:
    case 48:  // key:0 meaning:NONE
      break;
    case EAST:
    case 49:  // key:1 meaning:EAST
      train_state.instruction.current_instruction = O_EAST;
      break;
    case WEST:
    case 50:  // key:2 meaning:WEST
      train_state.instruction.current_instruction = O_WEST;
      break;
    case START:
    case 51:  // key:3 meaning:START
      train_state.instruction.current_instruction = O_START;
      break;
    case STOP:
    case 52:  // key:4 meaning:STOP
      train_state.instruction.current_instruction = O_STOP;
      break;
    case OPEN:
    case 53:  // key:5 meaning:OPEN
      train_state.instruction.current_instruction = O_OPEN;
      break;
    case CLOSE:
    case 54:  // key:6 meaning:CLOSE
      train_state.instruction.current_instruction = O_CLOSE;
      break;
    case EMERGENCY:
    case 55:  // key:7 meaning:EMERGENCY
      train_state.instruction.current_instruction = O_EMERGENCY;
      do_emergency();
      break;


    // cases 98-101, temporary value for inputs via serial to emulate colour detection
    case 97:  // key:a meaning:CS_NONE
      train_state.colour_sensor.last_colour = train_state.colour_sensor.current_colour;
      train_state.colour_sensor.current_colour = CS_NONE;
      train_state.colour_sensor.colour_updated = true;
      break;
    case 98:  // key:b meaning:CS_RED
      train_state.colour_sensor.last_colour = train_state.colour_sensor.current_colour;
      train_state.colour_sensor.current_colour = CS_RED;
      train_state.colour_sensor.colour_updated = true;
      break;
    case 99:  // key:c meaning:CS_GREEN
      train_state.colour_sensor.last_colour = train_state.colour_sensor.current_colour;
      train_state.colour_sensor.current_colour = CS_GREEN;
      train_state.colour_sensor.colour_updated = true;
      break;
    case 100:  // key:d meaning:CS_BLUE
      train_state.colour_sensor.last_colour = train_state.colour_sensor.current_colour;
      train_state.colour_sensor.current_colour = CS_BLUE;
      train_state.colour_sensor.colour_updated = true;
      break;
    case 101:  // key:e meaning:CS_YELLOW
      train_state.colour_sensor.last_colour = train_state.colour_sensor.current_colour;
      train_state.colour_sensor.current_colour = CS_YELLOW;
      train_state.colour_sensor.colour_updated = true;
      break;

    default:
      if (debug_mode) {
        Serial.println("ERROR - update_operator_instruction() - reading instruction");
        Serial.println(new_instruction);
      }
      break;
  }

}


/********************************************************************************/
/********************************************************************************/
/**********************    VALIDATE NEXT TRANSITION    **************************/
/********************************************************************************/
/********************************************************************************/

bool ready_next_state() {  
  if (train_state.instruction.current_instruction == O_START){
    if (train_state.train_door.current_state == DOOR_CLOSE) {
      train_state.motor.next_state = M_START;
      train_state.motor.next_speed = M_SLOW;
      train_state.motor.target_speed = M_SLOW;
    }    
  }
  
  // If colour sensor detects a change
  if (train_state.colour_sensor.colour_updated == true) {
    switch (train_state.colour_sensor.current_colour) {
      case CS_NONE:
        // no colour, no state change
        break;
      case CS_RED:
      //////////_1
      // Train is at the station
      // Check if the operator has sent STOP message 
      // or EAST/WEST requiring a change in direction
      
        // ensure action/change only happens once, until new patch of red
        //if (train_state.colour_sensor.colour_updated == false) {break;}
        train_state.colour_sensor.colour_updated = false;
        
        switch(train_state.instruction.current_instruction) {
          case O_STOP:
            train_state.motor.next_state = M_STOP;
            train_state.motor.next_speed = M_IDLE;
            train_state.motor.target_speed = M_IDLE;
  
            train_state.instruction.current_instruction = O_NONE;

          case O_EAST:
            if (train_state.motor.current_direction != M_EAST) {
              train_state.motor.next_direction = M_EAST;
              
              train_state.motor.next_state = M_STOP;
              train_state.motor.next_speed = M_IDLE;
              train_state.motor.target_speed = M_SLOW;  // makes train auto start after changing direction
              
            }
            break;
          case O_WEST:
            if (train_state.motor.current_direction != M_WEST) {
              train_state.motor.next_direction = M_WEST;

              train_state.motor.next_state = M_STOP;
              train_state.motor.next_speed = M_IDLE;
              train_state.motor.target_speed = M_SLOW;  // makes train auto start after changing direction
              
            }
            break;
          default:
            break;
        }
        break;
      case CS_GREEN:
      //////////_1
      // Change speed
        // ensure speed change only happens once, until new patch of green
        if (train_state.colour_sensor.colour_updated == false) {break;}
        train_state.colour_sensor.colour_updated = false;
        
        switch (train_state.motor.current_speed) {
          case M_SLOW:
            train_state.motor.next_speed = M_FAST;
            train_state.motor.target_speed = M_FAST;
            break;
          case M_FAST:
            train_state.motor.next_speed = M_SLOW;
            train_state.motor.target_speed = M_SLOW;
            break;
          default:
            break;
        }
        break;
      case CS_BLUE:
      //////////_1
      
        // ensure speed change only happens once
        if (train_state.colour_sensor.colour_updated == false) {break;}
        train_state.colour_sensor.colour_updated = false;
        
        do_emergency();    
        break;
      case CS_YELLOW:
      //////////_1
      // Change direction
        // ensure change only happens once, until new patch of yellow
        if (train_state.colour_sensor.colour_updated == false) {break;}
        train_state.colour_sensor.colour_updated = false;
        
        switch(train_state.motor.current_direction) {
          case M_EAST:
            train_state.motor.next_direction = M_WEST;
            // motor.next_speed is handled below
            train_state.motor.target_speed = train_state.motor.current_speed;
            break;
          case M_WEST:
            train_state.motor.next_direction = M_EAST;
            // motor.next_speed is handled below
            train_state.motor.target_speed = train_state.motor.current_speed;
            break;
          default:
            break;
        }
        break;
      default: 
      //////////_1
        break;
    }
  }

  
  // When train is idle at a station, with operator instruction
  if(train_state.motor.current_state == M_STOP && train_state.colour_sensor.last_colour == CS_RED) {
    switch (train_state.instruction.current_instruction) {
      case O_START:
        if (train_state.train_door.current_state == DOOR_CLOSE) {
          train_state.motor.next_state = M_START;
          train_state.motor.next_speed = M_SLOW;
          train_state.motor.target_speed = M_SLOW;
        }
        break;
      case O_EAST:
        train_state.motor.next_direction = M_EAST;

        train_state.motor.next_state = M_START;
        train_state.motor.next_speed = M_SLOW;
        train_state.instruction.current_instruction = O_NONE;
        break;
      case O_WEST:
        train_state.motor.next_direction = M_WEST;

        train_state.motor.next_state = M_START;
        train_state.motor.next_speed = M_SLOW;
        train_state.instruction.current_instruction = O_NONE;
        break;
      case O_OPEN:
        train_state.train_door.next_state = DOOR_OPEN;
        break;
      case O_CLOSE:
        train_state.train_door.next_state = DOOR_CLOSE;
        break;
      default:
        break;
    }
  }


  

  // Check if train is in process of changing direction
  // For YELLOW sensor where FAST EAST/WEST to FAST in the opposite direction
  // requiring multiple state transitions
  
  if (train_state.motor.current_direction != train_state.motor.next_direction) {
    ///// train is in the wrong direction, needs to slow down
    switch (train_state.motor.current_speed) {
      case M_FAST:
        train_state.motor.next_state = M_START;
        train_state.motor.next_speed = M_SLOW;
        break;
      case M_SLOW:
        train_state.motor.next_state = M_STOP;
        train_state.motor.next_speed = M_IDLE;
        break;
      case M_IDLE:  
        // train has stopped, direction can be changed on next transition
        break;
      default:
        break;
    }
  } 
  ///// Train is going in the correct direction, check that train is going at the correct speed
  if (train_state.motor.current_speed != train_state.motor.target_speed 
  && train_state.motor.current_direction == train_state.motor.next_direction) {
    
    switch (train_state.motor.target_speed) {
      case M_FAST:
        switch (train_state.motor.current_speed) {
          case M_SLOW:  // SLOW -> FAST
            train_state.motor.next_state = M_START;
            train_state.motor.next_speed = M_FAST;
            break;
          case M_IDLE:  // IDLE -> SLOW -> FAST
            train_state.motor.next_state = M_START;
            train_state.motor.next_speed = M_SLOW;
            break;
          default:
            break;
        }
        break;
      case M_SLOW:
        switch (train_state.motor.current_speed) {
          case M_FAST:  // FAST -> SLOW
            train_state.motor.next_state = M_START;
            train_state.motor.next_speed = M_SLOW;
            break;
          case M_IDLE:  // IDLE -> SLOW
            train_state.motor.next_state = M_START;
            train_state.motor.next_speed = M_SLOW;
            break;
          default:
            break;
        }
        break;
      case M_IDLE:
        switch (train_state.motor.current_speed) {
          case M_FAST:  // FAST -> IDLE
            train_state.motor.next_state = M_START;
            train_state.motor.next_speed = M_SLOW;
            break;
          case M_SLOW:  // SLOW -> IDLE
            train_state.motor.next_state = M_STOP;
            train_state.motor.next_speed = M_IDLE;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }

}


/********************************************************************************/
/********************************************************************************/
/**********************    PERFORM NEXT TRANSITION    ***************************/
/********************************************************************************/
/********************************************************************************/

bool do_state_transition() {
  bool success = false;
  bool starting = false;  // band-aid solution to check if train is gonig from IDLE -> SLOW

  
  ///// motor
  // speed & state
  if (train_state.motor.current_speed != train_state.motor.next_speed) {
    switch (train_state.motor.next_speed) {
      case M_FAST:
        success = motor_fast(&train_state.motor);
        if (success) {
          train_state.motor.current_speed = M_FAST;
          train_state.motor.current_state = M_START;
        }
        break;
      case M_SLOW:
        // band-aid solution to check if train is gonig from IDLE -> SLOW
        if (train_state.motor.current_speed == M_IDLE) {starting = true;}
        
        success = motor_slow(&train_state.motor);
        
        if (success) {
          train_state.motor.current_speed = M_SLOW;
          train_state.motor.current_state = M_START;

          // start instruction
          if (train_state.instruction.current_instruction == O_START) {
            //train_state.instruction.current_instruction = O_NONE;
            //send_go();
          }

          // starting
          if (starting) {      
            train_state.instruction.current_instruction = O_NONE;
            send_go();
          }
          
        }
        break;
      case M_IDLE:
        success = motor_stop(&train_state.motor);
        if (success) {
          train_state.motor.current_speed = M_IDLE;
          train_state.motor.current_state = M_STOP;
          
          send_stop();

          // change direction if needed
          train_state.motor.current_direction = train_state.motor.next_direction;

          // direction has changed due to yellow, send update to control box
          if (train_state.colour_sensor.last_colour == CS_YELLOW) {
            switch (train_state.motor.current_direction) {
              case M_EAST:
                send_east();
                break;
              case M_WEST:
                send_west();
                break;
              default:
                break;
            }
          }
          
          // direction has changed due to east/west instruction, send update to control box
          switch (train_state.instruction.current_instruction){
            case O_WEST:
              send_west();
              break;
            case O_EAST:
              send_east();
              break;
            default:
              break;
          }          
        }
        break;
      default:
        break;
    }
  }

  
  
  ///// door
  success = false;
  switch(train_state.train_door.next_state) {
    case DOOR_OPEN:
      success = open_door(&train_state.train_door);
      if (success) {
        train_state.train_door.current_state = train_state.train_door.next_state;
        if (train_state.instruction.current_instruction == O_CLOSE) {
          send_doors_open();
          train_state.instruction.current_instruction = O_NONE;
        }
      }
      break;
    case DOOR_CLOSE:
      success = close_door(&train_state.train_door);
      if (success) {
        train_state.train_door.current_state = train_state.train_door.next_state;
        if (train_state.instruction.current_instruction == O_CLOSE) {
          send_doors_closed();
          train_state.instruction.current_instruction = O_NONE;
        }        
      }
      break;
    default:
      break;
  }

}


/********************************************************************************/
/********************************************************************************/
/**************************    OTHER FUNCTION   *********************************/
/********************************************************************************/
/********************************************************************************/

void train_init() {
}


void do_emergency() {
}




/********************************************************************************/
/********************************************************************************/
/***********************    PRINT DEBUG MESSAGES    *****************************/
/********************************************************************************/
/********************************************************************************/

void print_counter() {
  p_counter += 1;
  Serial.println("--------------------------");
  Serial.println(p_counter);
}

void print_direction() {
  Serial.print("DIRECTION__:  ");
  switch (train_state.motor.current_direction) {
    case M_EAST:
      Serial.println("EAST");
      break;
    case M_WEST:
      Serial.println("WEST");
      break;
    default:
      Serial.println("DEFAULT");
      break;
  }
}

void print_speed() {
  Serial.print("SPEED______:  ");
  switch (train_state.motor.current_speed) {
    case M_IDLE:
      Serial.println("IDLE");
      break;
    case M_SLOW:
      Serial.println("SLOW");
      break;
    case M_FAST:
      Serial.println("FAST");
      break;
    default:
      Serial.println("DEFAULT");
      break;
  }
}


void print_colour() {
  Serial.print("COLOUR_____:  ");
  switch (train_state.colour_sensor.current_colour) {
    case CS_NONE:
      Serial.println("NONE");
      break;
    case CS_RED:
      Serial.println("RED");
      break;
    case CS_GREEN:
      Serial.println("GREEN");
      break;
    case CS_BLUE:
      Serial.println("BLUE");
      break;
    case CS_YELLOW:
      Serial.println("YELLOW");
      break;
    default:
      Serial.println("DEFAULT");
      break;
  }
}

void print_door() {
  Serial.print("DOOR_______:  ");
  switch (train_state.train_door.current_state) {
    case DOOR_OPEN:
      Serial.println("OPEN");
      break;
    case DOOR_CLOSE:
      Serial.println("CLOSE");
      break;
    default:
      Serial.println("DEFAULT");
      break;
  }
}

void print_motor() {
  Serial.print("MOTOR______:  ");
  switch (train_state.motor.current_state) {
    case M_STOP:
      Serial.println("STOPPED");
      break;
    case M_START:
      Serial.println("MOVING");
      break;
    default:
      Serial.println("DEFAULT");
      break;
  }
}

void print_instruction() {
  Serial.print("INSTRUCTION:  ");
  switch (train_state.instruction.current_instruction) {
    case O_NONE:
      Serial.println("O_NONE");
      break;
    case O_EAST:
      Serial.println("O_EAST");
      break;
    case O_WEST:
      Serial.println("O_WEST");
      break;
    case O_START:
      Serial.println("O_START");
      break;
    case O_STOP:
      Serial.println("O_STOP");
      break;
    case O_OPEN:
      Serial.println("O_OPEN");
      break;
    case O_CLOSE:
      Serial.println("O_CLOSE");
      break;
    case O_EMERGENCY:
      Serial.println("O_EMERGENCY");
      break;
    default:
      Serial.println("DEFAULT");
      break;
  }
}
