

/*
 * 
 */
void doCurrentAction() {
  switch(trainState.currentAction) {
    case NO_CHANGE:
      break;
    case SPEED_UP:
      // motor.accelerate
      break;
    case SLOW_DOWN:
      // motor.deccelerate
      break;
    case A_START:
      // motor.start
      break;
    case A_STOP:
      // motor.stop
      break;
    case A_EMERGENCY:
      // motor.brake
      break;
    default:
      break;
  }
  
}

/*
 * Update next action of the train based on its target and whether the current action is complete
 */
void updateNextAction() {
  // current actionn is done, but target action is not yet done
  if (trainState.currentActionComplete == true && trainState.targetActionComplete == false) {

    // check target action
    switch(trainState.targetAction) {
      // for target action, check next step (where multi-steps apply)
      case NO_CHANGE:
        break;
      case SPEED_UP:
        // train has reached target speed
        break;
      case SLOW_DOWN:
        break;
      case A_START:
        break;
      case A_STOP:
        break;
      case A_EMERGENCY: 
        break;
      case CHANGE_DIRECTION:
        // if CHANGE_DIRECTION, check current action to determine next action
        switch (trainState.currentAction) {
          case SPEED_UP:
            // assuming change directions are only acted at reaching a station or yellow line
            // so once the train has accelerated to speed, it is now going in the 'new' direction
            trainState.targetActionComplete = true;
            trainState.currentAction = NO_CHANGE;
            break;
            
          case SLOW_DOWN:
            // train has slowed down, next step is to stop
            trainState.currentAction = A_STOP;
            break;
            
          case A_STOP:
            // train has stopped
            // change the direction train should travel
            // next step is to start moving
            trainState.currentAction = A_START;
            
            switch(trainState.trainDirection) {
              case T_EAST:
                trainState.trainDirection = T_WEST;
                break;
              case T_WEST:
                trainState.trainDirection = T_EAST;
                break;
            }
            break;
            
          case A_START:
            // train has started moving
            // next step is to speed up
            trainState.currentAction = SPEED_UP;
            break;
            
          case A_EMERGENCY:
            trainState.currentAction = A_EMERGENCY;
            break;
            
          case CHANGE_DIRECTION:
            break;
            
          case NO_CHANGE:
            break;
            
          default:
            break;
        }
      
      
    }
  }
  
}


/*
 * Set the new action for the train. 
 * Update the state machine with the next/target action of the train
 * 
 * @param int newTrainAction - int value representing next action. Value given by TrainMovement.h
 */
void changeTrainTargetAction(int newTrainAction) {
  trainState.targetActionComplete = false;
  switch(newTrainAction) {
    case 0:
      trainState.currentAction = NO_CHANGE;
      break;
    case 1:
      trainState.currentAction = CHANGE_DIRECTION;
      break;
    case 2:
      trainState.currentAction = SPEED_UP;
      break;
    case 3:
      trainState.currentAction = SLOW_DOWN;
      break;
    case 4:
      trainState.currentAction = A_STOP;
      break;
    case 5:
      trainState.currentAction = A_START;
      break;
    case 6:
      trainState.currentAction = A_EMERGENCY;
      break;
    default:
      trainState.currentAction = NO_CHANGE;
      break;
  }   
}


void updateTrainCurrentAction() {
  // steps/process for changing direction:
  // SLOW_DOWN > A_STOP > change trainState.trainDirection > A_START > SPEED_UP

  // check current action to determine next action in CHANGE_DIRECTION process
  switch(trainState.currentAction) {
    case SPEED_UP:
      // assuming change directions are only acted at reaching a station or yellow line
      // so once the train has accelerated to speed, it is now going in the 'new' direction
      trainState.targetActionComplete = true;
      trainState.currentAction = NO_CHANGE;
      break;
      
    case SLOW_DOWN:
      // train has slowed down, next step is to stop
      trainState.currentAction = A_STOP;
      break;
      
    case A_STOP:
      // train has stopped
      // change the direction train should travel
      // next step is to start moving
      trainState.currentAction = A_START;
      
      switch(trainState.trainDirection) {
        case T_EAST:
          trainState.trainDirection = T_WEST;
          break;
        case T_WEST:
          trainState.trainDirection = T_EAST;
          break;
      }
      break;
      
    case A_START:
      // train has started moving
      // next step is to speed up
      trainState.currentAction = SPEED_UP;
      
    case A_EMERGENCY:
      trainState.currentAction = A_EMERGENCY;
  
    case NO_CHANGE:
    case CHANGE_DIRECTION:
    default:
      break;
  }  
}

/*
 * Check the if the train has achieved the target action
 */
void updateTrainTargetAction() {
  switch(trainState.targetAction) {
    case NO_CHANGE:
      break;
    case SPEED_UP:
      if (withinTolerance(trainState.currentSpeed, trainConstants.maxSpeed, trainConstants.speedTolerance)) {
        trainState.targetActionComplete = true;
        trainState.currentActionComplete = true;
      }
      break;
    case SLOW_DOWN:
      if (withinTolerance(trainState.currentSpeed, trainConstants.minSpeed, trainConstants.speedTolerance)) {
        trainState.targetActionComplete = true;
        trainState.currentActionComplete = true;
      }
    case A_START:
      if (withinTolerance(trainState.currentSpeed, trainConstants.minSpeed, trainConstants.speedTolerance)) {
        trainState.targetActionComplete = true;
        trainState.currentActionComplete = true;
      }
      break;
    case A_STOP:
      if (trainState.currentSpeed == 0) {
        trainState.targetActionComplete = true;
        trainState.currentActionComplete = true;
      }
      break;
    case A_EMERGENCY:
      // TODO
      updateNextAction();
      break;

    case CHANGE_DIRECTION:
      // final step in the CHANGE_DIRECTION process is to SPEED_UP
      // assumed change direction action takes place at either RED or YELLOW
      if (trainState.currentAction == SPEED_UP && trainState.currentActionComplete == true) {
        trainState.currentActionComplete = true;        
      }
    
    default:
      break;
  }
}
