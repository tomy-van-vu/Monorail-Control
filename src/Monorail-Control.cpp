#include <Arduino.h>

#include "Brakes.h"
#include "Comms.h"
#include "Colour_sensor.h"
#include "MetroDoor.h"
#include "Motor.h"


#include "BTInterface.h"
bt_interface bt_i = {4, "INIT", &Serial3};
#include "Helpers.h"


#define BAUD_RATE     9600
#define BLE_PIN       15
#define MOTOR_PIN     9
#define COLOUR_IN_S2  A4
#define COLOUR_IN_S3  A3
#define COLOUR_OUT_1  A5
#define DOOR_PIN      10
#define SOLENOID_PIN  5

#define EMERGENCY_PING_TIMER 1000 // ms, interval to transmite emergency signal to the control box


typedef struct {
  SM_colour_sensor colour_sensor;
  SM_motor motor;
  SM_door door;
  SM_instructions instruction;
} SM_train;

// SM declarations
SM_colour_sensor colour_sensor = {CS_NONE, CS_NONE, false};
SM_motor motor = {M_STOP, M_STOP, M_IDLE, M_IDLE, M_IDLE, M_WEST, M_WEST, 0};
SM_door door = {DOOR_CLOSE, DOOR_CLOSE};
SM_instructions instructions = {O_NONE, O_NONE, false};
SM_train train_state = {colour_sensor, motor, door, instructions};


// function declaration.
void do_emergency();
void do_state_transition();
void ready_next_state();
void train_init();
void update_operator_instruction(message);

// Read state machine, print current values for debugging/testin
void print_colour();
void print_counter();
void print_direction();
void print_door();
void print_instruction();
void print_motor();
void print_speed();


/********************************************************************************/
// Testing stuffs

//#define NO_COLOUR_SENSOR      // comment out when running WITHOUT> sensor connected to the board
//#define SERIAL_MONITOR_INPUT  // comment out when <NOT> accepting Serial monitor inputs
//#define DEBUG_MODE  true      // comment out for final upload

#ifdef DEBUG_MODE
unsigned long print_timer = millis();
unsigned int print_period = 4000;
int p_counter = 0;
#endif

/********************************************************************************/
/********************************************************************************/
/********************************    SETUP    ***********************************/
/********************************************************************************/
/********************************************************************************/

void setup() {
  Serial.begin(BAUD_RATE);
  Serial3.begin(BAUD_RATE);
  
  
  pinMode(BLE_PIN, INPUT);
  colour_sensor_init(COLOUR_IN_S2, COLOUR_IN_S3, COLOUR_OUT_1);
  Motor_init(MOTOR_PIN);
  MetroDoor_init(DOOR_PIN);


  delay(2000);

  send_west();
  send_doors_closed();
  send_stop();
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
  // function stalls/takes ~2100ms when not receiving any input data
  #ifndef NO_COLOUR_SENSOR
  sensor_colour c = check_colour_sensor(&colour_sensor);

  // Colour sensor function is not updating sensor states with given pointer
  // hotfix solution - sensor function passes back colour (as enum sensor_colour)
  if(c != train_state.colour_sensor.current_colour) {
    train_state.colour_sensor.last_colour = train_state.colour_sensor.current_colour;
    train_state.colour_sensor.current_colour = c;
    train_state.colour_sensor.colour_updated = true;
  }
  #endif
  
  // update state machine
  ready_next_state();
  do_state_transition();

  // TODO: split transmit update to control-box into separate function
  // currently scattered throughout do_state_transition()


  // DEBUG
  #ifdef DEBUG_MODE
  if (millis() - print_timer >= print_period) {
    print_timer = millis();
    
    print_counter();
    print_instruction();  // current instruction from operator to be executed
    print_colour();       // most recent colour from the sensor
    print_motor();        // whether motor is stopped or moving
    print_speed();        // fast, slow, stopped/idle
    print_direction();    // east or west
    print_door();         // open or closed

    Serial.println();

  }
  #endif
  
  
}


/********************************************************************************/
/********************************************************************************/
/*******************    STORE OPERATOR'S INSTRUCTION    *************************/
/********************************************************************************/
/********************************************************************************/

/*
 * 
 */
void update_operator_instruction(message new_instruction) {
  switch (new_instruction) {
    case NONE:
      train_state.instruction.current_instruction = O_NONE;
      break;
    case EAST:
      train_state.instruction.current_instruction = O_EAST;
      break;
    case WEST:
      train_state.instruction.current_instruction = O_WEST;
      break;
    case START:
      train_state.instruction.current_instruction = O_START;
      break;
    case STOP:      
      train_state.instruction.current_instruction = O_STOP;
      break;
    case OPEN:
      train_state.instruction.current_instruction = O_OPEN;
      break;
    case CLOSE:
      train_state.instruction.current_instruction = O_CLOSE;
      break;
    case EMERGENCY:
      train_state.instruction.current_instruction = O_EMERGENCY;
      do_emergency();
      break;
    default:
      #ifdef DEBUG_MODE
      #ifndef ALLOW_SERIAL_MONITOR_INPUT  // stop test inputs from Serial Moniotr from triggering error message
      Serial.println("update_operator_instruction() - Received non operator instruction");
      Serial.print("Received message:  ");
      Serial.println(new_instruction);
      #endif
      #endif
      break;

  }
  
  //////
  // Single char values for inputs via serial monitor to emulate data inputs from sensors 
  // and peripherals. For testing without hardware
  #ifdef ALLOW_SERIAL_MONITOR_INPUT
  switch ((int)new_instruction) {
    case 48:  // key:0 meaning:NONE
      train_state.instruction.current_instruction = O_NONE;
      break;
    case 49:  // key:1 meaning:EAST
      train_state.instruction.current_instruction = O_EAST;
      break;
    case 50:  // key:2 meaning:WEST
      train_state.instruction.current_instruction = O_WEST;
      break;
    case 51:  // key:3 meaning:START
      train_state.instruction.current_instruction = O_START;
      break;
    case 52:  // key:4 meaning:STOP
      train_state.instruction.current_instruction = O_STOP;
      break;
    case 53:  // key:5 meaning:OPEN
      train_state.instruction.current_instruction = O_OPEN;
      break;
    case 54:  // key:6 meaning:CLOSE
      train_state.instruction.current_instruction = O_CLOSE;
      break;
    case 55:  // key:7 meaning:EMERGENCY
      train_state.instruction.current_instruction = O_EMERGENCY;
      do_emergency();
      break;


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

    case 102:  // key:f
      send_stop();
      break;
    case 103:  // key:g
      send_go();
      break;
    case 104:  // key:h
      send_east();
      break;
    case 105:  // key:i
      send_west();
      break;
    case 106:  // key:j
    send_doors_open();
      break;
    case 107:  // key:k
      send_doors_closed();
      break;

    case 2:   // STX
    case 3:   // ETX
    case 4:   // EOT
    case 5:   // ENQ
    case 6:   // ACK  
      break;
    default:
      #ifdef DEBUG_MODE
      Serial.println("--------------------------");
      Serial.println("ERROR - update_operator_instruction() - read invalid instruction");
      Serial.print("Instruction ASCII value:  ");
      Serial.println(new_instruction);
      Serial.println("--------------------------");
      break;
      #endif
  }
  #endif

}


/********************************************************************************/
/********************************************************************************/
/****************    VALIDATE AND SETUP NEXT TRANSITION    **********************/
/********************************************************************************/
/********************************************************************************/

void ready_next_state() {  

  /////
  // If colour sensor detects a change in colour
  if (train_state.colour_sensor.colour_updated == true) {
    switch (train_state.colour_sensor.current_colour) {
      case CS_NONE:
        // no colour, no state change
        break;
      case CS_RED:
      //////////
      // Train is at the station
      // Check if the operator has sent STOP or 
      // EAST/WEST message that requires a change in direction
        if (train_state.colour_sensor.colour_updated == false) {break;}
        train_state.colour_sensor.colour_updated = false;
        
        switch(train_state.instruction.current_instruction) {
          case O_STOP:
            train_state.motor.next_state = M_STOP;
            train_state.motor.next_speed = M_IDLE;
            train_state.motor.target_speed = M_IDLE;
            break;

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
      //////////
      // Change speed
        // ensure speed change only happens once on the same patch of green
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
      //////////
        if (train_state.colour_sensor.colour_updated == false) {break;}
        train_state.colour_sensor.colour_updated = false;
        do_emergency();    
        break;
      case CS_YELLOW:
      //////////
      // Change direction
        // ensure change only happens once, until new patch of yellow
        if (train_state.colour_sensor.colour_updated == false) {break;}
        train_state.colour_sensor.colour_updated = false;
        
        switch(train_state.motor.current_direction) {
          case M_EAST:
            train_state.motor.next_direction = M_WEST;
            train_state.motor.target_speed = train_state.motor.current_speed;
            break;
          case M_WEST:
            train_state.motor.next_direction = M_EAST;
            train_state.motor.target_speed = train_state.motor.current_speed;
            break;
          default:
            break;
        }
        break;
      default: 
      //////////
        Serial.println("ERROR - ready_next_state() - read non valid colour state");
        break;
    }
  }

  /////
  // When train is stopped
  if(train_state.motor.current_state == M_STOP){
//  && (train_state.colour_sensor.last_colour == CS_RED || train_state.colour_sensor.current_colour == CS_RED)) { // Accomodate for possible positioning of colour sensor on the train
    switch (train_state.instruction.current_instruction) {
      case O_START: // Door must be closed for train to start moving
        if (train_state.door.current_state == DOOR_CLOSE) {
          train_state.motor.next_state = M_START;
          train_state.motor.next_speed = M_SLOW;
          train_state.motor.target_speed = M_SLOW;
        }
        break;
      case O_EAST:
        train_state.motor.next_direction = M_EAST;
        train_state.instruction.current_instruction = O_NONE;
        break;
      case O_WEST:
        train_state.motor.next_direction = M_WEST;
        train_state.instruction.current_instruction = O_NONE;
        break;
      case O_OPEN:
        train_state.door.next_state = DOOR_OPEN;
        break;
      case O_CLOSE:
        train_state.door.next_state = DOOR_CLOSE;
        break;
      default:
        break;
    }
  }


  
  /////
  // Train should be going in the opposite direction
  // Slow down, then stop 
  
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


  /////
  // Speed correction
  // Train is going in the correct direction
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

void do_state_transition() {
  // TODO: split tx into own function
  // States to send update to control box:
    // Starting to move
    // Stopping
    // Changing direction
    // Door opening
    // Door closing
  
  bool success = false;
  bool starting = false;  // band-aid solution to check if train is gonig from IDLE -> SLOW

  
  ///// Motor states
  // speed & stopped/moving transitions
  if (train_state.motor.current_speed != train_state.motor.next_speed) {
    switch (train_state.motor.next_speed) {
      case M_FAST:
        success = motor_fast(&train_state.motor);
        if (success) {
          train_state.motor.current_speed = train_state.motor.next_speed;
          train_state.motor.current_state = train_state.motor.next_state;
        }
        break;
      case M_SLOW:
        // band-aid solution to check if train is going from IDLE -> SLOW
        if (train_state.motor.current_speed == M_IDLE) {starting = true;}
        
        success = motor_slow(&train_state.motor);
        if (success) {
          train_state.motor.current_speed = train_state.motor.next_speed;
          train_state.motor.current_state = train_state.motor.next_state;
          if (starting) {      
            send_go();
          }
          
        }
        break;
      case M_IDLE:
        success = motor_stop(&train_state.motor);
        if (success) {
          train_state.motor.current_speed = train_state.motor.next_speed;
          train_state.motor.current_state = train_state.motor.next_state;
          
          send_stop();

          // change direction if needed
          train_state.motor.current_direction = train_state.motor.next_direction;

          // direction has changed due to yellow, send update to control box
          if (train_state.colour_sensor.last_colour == CS_YELLOW || train_state.colour_sensor.current_colour == CS_YELLOW) {
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


  /////
  // Band-aid solution to overlooked state transition
  // Train is stopped, direction transition if needed
  if (train_state.motor.current_state == M_STOP) {
    if (train_state.motor.current_direction != train_state.motor.next_direction) {
      
      switch(train_state.motor.next_direction) {
        case M_EAST:
          send_east();
          break;
        case M_WEST:
          send_west();
          break;
        default:
          break;
      }
      train_state.motor.current_direction = train_state.motor.next_direction;
    }
  }
  
  /////
  // door
  if (train_state.door.current_state != train_state.door.next_state){
    success = false;
    switch(train_state.door.next_state) {
      case DOOR_OPEN:
        success = open_door();
        if (success) {
          train_state.door.current_state = train_state.door.next_state;
          if (train_state.instruction.current_instruction == O_OPEN) {
            send_doors_open();
            train_state.instruction.current_instruction = O_NONE;
          }
        }
        break;
      case DOOR_CLOSE:
        success = close_door();
        if (success) {
          train_state.door.current_state = train_state.door.next_state;
          if (train_state.instruction.current_instruction == O_CLOSE) {
            send_doors_closed();
          }        
        }
        break;
      default:
        break;
    }    
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
  // TODO: engage e-brakes instead of using motor to stop
  bool success = false;

  // force stop via aggressive motor control
  // pulse brakes until stopped
  // Transmit emergency signal to control box periodically
  unsigned long emergency_last_tx = millis();
  while (true) {
    if (!success) {
      success = motor_brake(&train_state.motor);
      brakes_pulse();
    }
    if (millis() - emergency_last_tx >= EMERGENCY_PING_TIMER) {
      send_emergency();
      emergency_last_tx = millis();
    }
  }
}



/********************************************************************************/
/********************************************************************************/
/***********************    PRINT DEBUG MESSAGES    *****************************/
/********************************************************************************/
/********************************************************************************/

#ifdef DEBUG_MODE
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
  switch (train_state.door.current_state) {
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

#endif