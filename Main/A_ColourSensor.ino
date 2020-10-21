
/*
 * Logic to detect when there is a new actionable colour detected
 * 
 * @param int sensorReading - colour value from the colour sensor
 * 
 * @return boolean - newColour detected
 */
boolean detectNewColour(int sensorReading) {
  // invalid colour
  if (sensorReading >4 || sensorReading <0) {return false;}
  
  // no colour, do nothing
  if (sensorReading == NO_COLOUR) {return false;}
  
  // no change in colour, do nothing
  if (trainState.lastSensorColour == sensorReading) {return false;}

  // If NO_COLOUR is followed by any colour reading
  // i.e. the sensor is not reading the same band/patch of colour on the track repeatedly
  // e.g. GREEN > NO_COLOUR > GREEN
  if (trainState.lastSensorColour == NO_COLOUR) {
    trainState.colourChanged = true;
    return true;
  }

  // Colour change between adjacent bands
  // i.e. there is no empty band between two patches of colour
  // e.g. YELLOW > BLUE
  if (trainState.lastSensorColour != NO_COLOUR 
      && trainState.lastSensorColour != trainState.lastActionedColour) {
    trainState.colourChanged = true;
    return true;
  }
  return false;
}



/* 
 * Helper function
 * 
 * Takes a colour value given by ColourSensor.h to update state machine
 * 0 = no colour
 * 1 = red
 * 2 = green
 * 3 = blue
 * 4 = yellow
 * 
 * @param int sensorReading - colour value from the colour sensor
 */
void updateColour(int sensorReading) {
  switch(sensorReading) {
    case 0:
      trainState.lastSensorColour = NO_COLOUR;
      break;
    case 1:
      trainState.lastSensorColour = RED;
      break;
    case 2:
      trainState.lastSensorColour = GREEN;
      break;
    case 3:
      trainState.lastSensorColour = BLUE;
      break;  
    case 4:
      trainState.lastSensorColour = YELLOW;
      break;
    default:
      trainState.lastSensorColour = NO_COLOUR;
      break;
  }    
}
