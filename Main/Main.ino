/*
 * Autonomous Train
 * LAST_UPDATED - 20 October 2020
 * 
 * ENGG 2/3k WedPM COMMS_4
 * 
 * __AUTHORS__
 * Vu, Van 
 * Bhanderi, Ashwin
 * Dhawan, Pulkit
 * Fricke, Ben
 * Garth, Richard
 * Jacob, Rachel
 */
 

#define BAUD_RATE 9600

/*
#define pin 
*/


#include <BTInterface.h>
bt_interface bt_i = {4, "INIT", &Serial2};
#include <Helpers.h>

//#include <Motor>
#include <ColourSensor.h>
/////////////// Accelerometer
#include <TrainMovement.h>

/*
 * 
 */



/*********************************************************************/
/****************************** Globals ******************************/
/*********************************************************************/
enum direction {T_EAST, T_WEST};
enum colour {NO_COLOUR, RED, GREEN, BLUE, YELLOW};
enum operatorInstructionSet {O_NONE, O_EAST, O_WEST, O_START, O_STOP, O_OPEN, O_CLOSE, O_EMERGENCY};
enum action {NO_CHANGE, CHANGE_DIRECTION, SPEED_UP, SLOW_DOWN, A_STOP, A_START, A_EMERGENCY};

//typedef
struct TrainState {
  enum direction trainDirection;   
  enum colour lastSensorColour;        
  enum colour lastActionedColour;
  int currentSpeed; 
  boolean trainMoving;   // may use currentSpeed to determine if moving
  boolean doorOpen;     
  boolean brakesActive;    
  boolean colourChanged;
  enum action currentAction;  // to track progress of changing directions
  enum action targetAction;
  boolean currentActionComplete;
  boolean targetActionComplete;
}trainState = {T_EAST, NO_COLOUR, NO_COLOUR, 0, false, false, false, false, NO_CHANGE, NO_CHANGE, true, true};

//typedef
struct OperatorInstructions {
  boolean hasUpdate;
  enum operatorInstructionSet lastInstruction; 
  enum operatorInstructionSet newInstruction;
}instructions = {false, O_NONE, O_NONE};

//typedef
struct TrainConstants {
  int maxSpeed;
  int minSpeed;         // min speed so train does not deccelerate into stopping at random location
  float speedTolerance;
  int accelerationTime; // time for train to go from stationary to maxSpeed (milliseconds)
}trainConstants = {100, 20, 0.1, 4000};

int currentSensorColour = -1;


/*********************************************************************/
/****************************** Testing ******************************/
/*********************************************************************/
boolean testingMode = true;
unsigned long printTime = millis();
unsigned long colourTime = millis();
unsigned long printDelay = 2000;  // print every 2 second
unsigned long colourDelay = 5000; // change colour every 5 seconds

/*********************************************************************/
/******************************** Setup ******************************/
/*********************************************************************/
void setup() {
  // initialise serial ports for BT
  Serial.begin(BAUD_RATE);
  initialise_interface(&bt_i);
  Serial2.begin(BAUD_RATE);

  
  // Initialise states
  // Connect Bluetooth
  // Intialise hardware
    // Colour sensors
    // Drive motors
    // Door motor
    // Brake motor
    // Accelerometer  
}


/*********************************************************************/
/****************************** Main Loop ****************************/
/*********************************************************************/
void loop() {
  if (millis() - printTime >= printDelay && testingMode == true) {
    printTime = millis();

    Serial.print("Sensor Colour:  ");
    Serial.println(currentSensorColour);

    Serial.print("Current Action:  ");
    Serial.println(trainState.currentAction);
    
    Serial.print("Target Action:  ");
    Serial.println(trainState.targetAction);

    Serial.print("Current Speed:  ");
    Serial.println(trainState.currentSpeed);

    Serial.println("--------------------------------------");
    Serial.println("--------------------------------------");
  }

  ////// Comms
  BTReceive();
  if (instructions.hasUpdate) {
    if (instructions.newInstruction == O_EMERGENCY) {
      emergencyBrake();
    } else {
      
    }
  }
  
  ////// Colour sensor
  // get colour sensor reading
  currentSensorColour = readColour();
  // check if its a new band of colour
  detectNewColour(currentSensorColour);
  // update colour sensor data
  updateColour(currentSensorColour);   


  ////// Movement logic
  // New colour detected, train needs to respond
  if (trainState.colourChanged) {
    int targetAction = movementDecisions(trainState.lastActionedColour, trainState.lastSensorColour);
    changeTrainTargetAction(targetAction);
  }

  // Check if target action is complete
  if (trainState.targetActionComplete == false) {
    doCurrentAction();
    updateNextAction();
    updateTrainCurrentAction();
    updateTrainTargetAction();
  }
  
    
  // Motor operation
  
}

/*********************************************************************/
/****************************** Functions ****************************/
/*********************************************************************/



/*
 * Process of checking and validating Bluetooth communication
 * Unpacks operator instructions
 */
boolean hasNewInstruction() {
  // Check Serial for Bluetooth communication
  // API and function calls provided by T4_C_2 group
  // Using placeholder function "BTReceive()"
  
//  if (BTReceive()) {
    // Decode data received to if it is communicaiton from the operator
    // or random noise
//    unpackData();
//    if(instructionUpdate) {
//      return true;
//    }
//  }
  return false;
}


void updateMotor(boolean direction, int speed, boolean acceleration) {
  // Motor code for operation
}
