/*
 * Autonomous Train - October 2020
 * 
 * ENGG 2/3k WedPM COMMS_4
 * 
 * __AUTHORS__
 * Bhanderi, Ashwin
 * Dhawan, Pulkit
 * Fricke, Ben
 * Garth, Richard
 * Jacob, Rachel
 * Vu, Van 
 */
 
// #include motor library by Ashwin

boolean testingMode = true;

// Globals 
volatile boolean instructionUpdate = false;
volatile boolean trainDirection = 0;

volatile boolean isMoving = 0;
volatile boolean acceleration = 0; // -1=deccelerate, 0=maintain speed, 1=accelerate
volatile int trainSpeed = 0;

volatile int sensorColor = 0; // e.g. range from 0-4 to denote colours


int maxSpeed = 100;
int accelerationTime = 4; // time for train to go from stationary to maxSpeed

void setup() {
  // put your setup code here, to run once:

  // Initialise states
  // Connect Bluetooth
  // Intialise hardware
    // Colour sensors
    // Drive motors
    // Door motor
    // Brake motor
    // Accelerometer  

  // wait for first instruction from the operator
  while (!hasNewInstruction()) {
    moveTrain();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
 
}

/*
 * Process of checking and validating Bluetooth communication
 * Unpacks operator instructions
 */
boolean hasNewInstruction() {
  // Check Serial for Bluetooth communication
  // API and function calls provided by T4_C_2 group
  // Using placeholder function "BTReceive()"
  
  if (BTReceive()) {
    // Decode data received to if it is communicaiton from the operator
    // or random noise
    unpackData();
    if(instructionUpdate) {
      return true;
    }
  }
  return false;
}

/*
 * Engages motor in given direction
 */
void moveTrain() {
  // Placeholder code
  // TODO
    // When changing direction, slow down then reverse motor rotation 
    // to avoid overloading
  
  // Speeding up
  if (acceleration && trainSpeed < maxSpeed) {
    updateMotor(trainDirection, trainSpeed, acceleration);

  // Slowing down
  } else if (!acceleration && trainSpeed > 0) {
    updateMotor(trainDirection, trainSpeed, acceleration);
  }
  
}

void updateMotor(boolean direction, int speed, boolean acceleration) {
  // Motor code for operation
}

/*
 * 
 */
void unpackData() {
}

/*
 * API and function calls provided by T4_C_2 group
 * Used as placeholder function
 */
boolean BTReceive() {
  /* 
   *  Consider checking that the incoming data packet is of the 
   *  correct size to avoid partial instructions
   *  
   *  This may happen due to processor executing instructions 
   *  faster than the baud rate (CPU is faster than BT transmission)
   */
  if (testingMode) {
    return true;
  }
  return false;
}
