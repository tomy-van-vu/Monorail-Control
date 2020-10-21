#include "Arduino.h"

/*
 * Return reference
 0 NO_CHANGE
 1 CHANGE_DIRECTION
 2 SPEED_UP
 3 SLOW_DOWN
 4 STOP
 5 START
 6 EMERGENCY
 */


/***************** Constructor *****************/
void TrainMovement() {
	// 
}



int movementDecisions(int lastColour, int newColour) {
	
	// brute solution
	// hard code action for each state combination


	switch(lastColour) {
		// lastColour RED -> newColour
		case 1:
			switch(newColour) {
				case 1:
					// RED, RED
					// NO_CHANGE
					return 0;
				case 2:
					// RED, GREEN
					// SPEED_UP
					return 2;
				case 3:
					// RED, BLUE
					// EMERGENCY
					return 6;
				case 4:
					// RED, YELLOW
					// CHANGE_DIRECTION
					return 1;
				default:
					return -1;
			}
			
		// lastColour GREEN -> newColour
		case 2:
			switch(newColour) {
				case 1:
					// GREEN, RED
					// STOP
					return 4;
				case 2:
					// GREEN, GREEN
					// SLOW_DOWN
					return 3;
				case 3:
					// GREEN, BLUE
					// EMERGENCY
					return 6;
				case 4:		
					// GREEN, YELLOW
					// CHANGE_DIRECTION
					return 1;
				default:
					return -1;
			}			
			
		// lastColour BLUE
		case 3:
			// EMERGENCY
			return 6;
			
		// lastColour YELLOW -> newColour
		case 4:
			// CHANGE_DIRECTION
			return 1;
		
		default:
			return -1;
	}
}







