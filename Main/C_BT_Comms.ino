/*
 * API and function calls provided by T4_C_2 group
 * Used as placeholder function
 */
boolean BTReceive() {
  int incomingInstruction;
  if (Serial2.available() >0) {
     incomingInstruction = Serial2.read();
     
     return validateMessage(incomingInstruction);
  }
  return false;
}


/*
 * 
 */
void unpackData() {
  
}


boolean validateMessage(int msg) {

  switch(msg) {
    case NONE:
      return false;
    case EAST:
      instructions.newInstruction = O_EAST;
      return true;
    case WEST:
      instructions.newInstruction = O_WEST;
      return true;
    case START:
      instructions.newInstruction = O_START;
      return true;
    case STOP:
      instructions.newInstruction = O_STOP;
      return true;
    case OPEN:
      instructions.newInstruction = O_OPEN;
      return true;
    case CLOSE:
      instructions.newInstruction = O_CLOSE;
      return true;
    case EMERGENCY:      
      instructions.newInstruction = O_EMERGENCY;
      return true;
    default:
      return false;
  }

  return false;
}
