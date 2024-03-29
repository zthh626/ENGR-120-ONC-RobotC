#pragma config(Sensor, in1,    motor_angle,    sensorPotentiometer)
#pragma config(Motor,  port1,           motor1,        tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**********************************************************
** bool set_motor( int target )
** Runs motor to try and hit target angle
** Inputs:
**   target  - target reading from potentiometer.
** Output:
**    true   - motor has reached desired position
**    false  - motor has not hit target position
**
** Sets motor output.
** Reads motor from port motor_angle
**********************************************************/
bool set_motor( int target )
{

	// Compute difference from current position and target position.
	int delta = target - SensorValue[motor_angle];
	int max = 15
	int acc;

	// Find if current position is lower or higher than current position.
	if ( delta > 0 ) {

	  // If current position is lower than target position, run motor with positive control.
		motor[motor1] = max;

		// Return false, indicating we are not at target position.
		return(false);

  } else if ( delta < 0 ) {

		// If current position is higher than target position, run motor with negative control.
		motor[motor1] = -max;

		// Return false, indicating we are not at target position.
		return(false);

  } else {
		// If motor is exactly at target position, turn motor off.
		motor[motor1] = 0;

		// Return true, indicating we are at target position.
		return(true);
	}
}

task main()
{
	// Target value to move motor towards.
	int target_value = 1500;

	// Time unitl motor hits target value.
	int first_time;
	// Maximum error after first time motor hits target value.
	int max_error;
	// Noted if motor hit target value.
	bool hit_target = false;

	int delta;

	// Reset timer.
	clearTimer(T1);

	// Run for either 5 seconds or until we get to target value
	while( time1(T1) < 5000 ) {

	  // Implements one iteration of control algorithm.
		set_motor( target_value );

		// If we haven't hit target value yet...
		if ( ! hit_target ) {
			// Check if we encounter target_value again.
			if ( SensorValue[motor_angle] == target_value ) {
				// Note hit target value.
				hit_target = true;

				// Record first time.
				first_time = time1(T1);

				// Reset maximum error value.
				max_error = 0;
			}
		} else {

		  // Find deviation from target value.
			delta = SensorValue[motor_angle] - target_value;

			// Find absolute value of deviation.
			if ( delta < 0 ) {
				delta = -1 * delta;
			}

			// If deviation is larger then previously found maximum deviation, update maximum deviation.
			if ( delta > max_error ) {
				max_error = delta;
			}

		}
	}//   match while( time1(T1) < 5000 )

	// Turn off motor
	motor[motor1] = 0;

	// Record results.
	if ( hit_target ) {
		writeDebugStream("First Time: %d\tMax Error: %d\n",first_time,max_error);
		} else {
		writeDebugStream("No convergence.\n");
	}

	// Infinite loop to keep DebugStream open so output can be read.
	while(1);
}
