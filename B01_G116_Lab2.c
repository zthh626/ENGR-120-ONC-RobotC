
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Sensor, dgtl2,  button2,        sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           motor1,        tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// WHEN YOU FINISH EACH EXERCISE, INCREMENT THIS VALUE
#define EXERCISE_NUMBER 	3

bool button1_pushed; //flag to store button1 input
bool button2_pushed; //flag to store button2 input
int x;
bool y = false;


/* monitorInput()
*
*  Used to flag button inputs
*       - this avoids errors caused by program recognizing input, taking action, and
*         reading input again before button is released
*/
void monitorInput()
{
  if(SensorValue(button1) && !button1_pushed)
  {
    button1_pushed = true;
  }

  if(SensorValue(button2) && !button2_pushed)
  {
    button2_pushed = true;
  }
}

void exercise_1()
{
	// Define tag for enumerated type for exercise 1
	enum T_exercise1_state {
		MOTOR_STOP = 0,
		MOTOR_RUNNING
	};

	// Declare variable to hold state, intialize to MOTOR_STOP state.
	T_exercise1_state exercise1_state = MOTOR_STOP;

	while(true)
	{
		// This function updates the button1_pushed and button2_pushed flags.
		monitorInput();

		// Switch the states.
		switch(exercise1_state) {

			// Code for MOTOR_STOP state:
		case MOTOR_STOP:
			// Turn motor off.
			motor[motor1] = 0;
			if ( button1_pushed ) {
				// If button1 pushed, change to the MOTOR_RUNNING state.
				exercise1_state = MOTOR_RUNNING;
				// Clear flag to indicate button 1 processed.
				button1_pushed = false;
			}
			break;
		case MOTOR_RUNNING:
			// Turn motor on.
			motor[motor1] = 50;
			if (button2_pushed) {
				// If button 2 pushed, transition to MOTOR_STOP state.
				exercise1_state = MOTOR_STOP;
				// Clear flag to indicate that button 2 processed.
				button2_pushed = false;
			}
			break;
		default:
		}

	}//end while
}

void exercise_2()
{
	enum T_test_state {
		state1 = 0,
		state2,
		state3
	};

	//Initializes state to state1
	T_test_state test_state = state1;

	//Starts loop to check for conditions
  while(true){
  	//Checks input
    monitorInput();

		//Switch statement for different states
    switch(test_state){
    //Initial off case
    case state1:
	    motor[motor1] = 0;
		  if (button1_pushed) {
		    test_state = state2;
		    button1_pushed = false;
		  }
    	break;

    //Case for motor to run
    case state2:
		  if (button1_pushed){
		  	// Sets encoder to 0
		    resetMotorEncoder(motor1);
		    // Loop till reaches target ticks
			  while( getMotorEncoder(motor1) < 627  ){
			    motor[motor1] = -50;
			  }
			  //Turns motor off after reaching 627 ticks
			  motor[motor1] = 0;
			  button1_pushed = false;
				}
			break;

  	}//switch
	}//while
}//exercise_2

void exercise_3()
{

	enum T_state{
		off = 0,
		forward,
		backward,
	};

	//initilizes state to off
	T_state state = off;

	//starts loop to check for conditions
	while(true){
		//checks for input
		monitorInput();

		//switch statement for state
		switch(state){

			//initial case/ off state, when motor is not moving
			case off:
				//always has motor speed to 0 and encoder to 0 unless button is pressed
				motor[motor1] = 0;
				resetMotorEncoder(motor1);
				//if button1 pressed state set to forward and button is reset
				if(button1_pushed){
					state = forward;
					button1_pushed = false;
				}
				//if button2 pressed state set to forward and button is reset
				if(button2_pushed){
					state = backward;
					button2_pushed = false;
				}
				break;

			//case for when motor is forward state
			case forward:
				//button1 will always be ignored and if in forward state, will always be moving forward
				button1_pushed = false;
				motor[motor1] = -50;

				//when encoder is less than 3000 and button 2 is not pushed, it will check for if button 2 is pressed
				if(getMotorEncoder(motor1) < 3000  && !y){
					//if true, sets y boolean to true
					if(button2_pushed){
						y = true;
						button2_pushed = false;
					}
				//if encoder is >= to 3000 and button 2 is not pressed, will work normally (terminate 3000)
				}else if(getMotorEncoder(motor1) >= 3000 && !y){
					motor[motor1] = 0;
					state = off;
					resetMotorEncoder(motor1);
				//if encoder greater than 3000 and button 2 is pushed
				} else if(getMotorEncoder(motor1) >= 3000 && y){
					//sets y to false, state to backward, and resets encoder
					y = false;
					state = backward;
					resetMotorEncoder(motor1);
				}
				break;

			case backward:
		  	//button2 will always be ignored and if in backward state, will always be moving backward
				button2_pushed = false;
				motor[motor1] = 50;

				//when encoder is more than -3000 and button 1 is not pushed, it will check for if button 1 is pressed
				if(-3000 < getMotorEncoder(motor1)  && !y){
					if(button1_pushed){
						y = true;
						button1_pushed = false;
					}
				//if encoder is < -3000 and button 1 is not pressed, will work normally (terminate -3000)
				}else if(-3000 > getMotorEncoder(motor1) && !y){
					motor[motor1] = 0;
					state = off;
					resetMotorEncoder(motor1);
				//if encoder less than -3000 and button 1 is pushed
				} else if(-3000 > getMotorEncoder(motor1) && y){
					//sets y to false, state to forward, and resets encoder
					y = false;
					state = forward;
					resetMotorEncoder(motor1);
				}
				break;
		}//switcj
	}//while
}//exercise_3

task main()
{
	button1_pushed = button2_pushed = false;

	switch (EXERCISE_NUMBER)
  {
     case 1:
       exercise_1();
       break;
     case 2:
       exercise_2();
       break;
     case 3:
       exercise_3();
       break;
     default: //should never get here.
   } // end switch

}// end main