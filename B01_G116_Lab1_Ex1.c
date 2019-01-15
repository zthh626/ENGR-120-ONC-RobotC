// Basic simulate imprecise measurement code in RobotC

// Upper and lower of range of values to estimate.
const int min_value = 0;
const int max_value = 100;

// Experiment parameters
int total_trials = 1000;

// Storage of number of guesses.
int total_meas = 0;
int correct_guess = 0;

/**********************************************************
** int resolve_measurement( int meas_point )
** Resolve Measurement at set point.
** Inputs:
**   meas_point	Set point of measurment
**              If meas_point is a non-negative number, the
**              function returns the value below.  If meas_point
**              is negative, the function generates a new random
**              value and returns this value.  This value should
**              only be used for debugging purposes.
** Outputs: (if input is non-negative)
**   0						If value is equal to meas_point
**   -1          If value is greater than meas_point
**   1           If true value is less than meas_pont
** Global Interaction:
**   Keeps track of number of measurements in total_meas
**
**********************************************************/
int resolve_measurement( int meas_point ) {

	// Storage of random number.
	//   static means value is maintained between function calls.
	static int true_value;

	// Check if new value is desired...
	if ( meas_point < 0 ) {
		// Random number parameter
		int temp_number = rand();

		int range = max_value - min_value;

		// Kludge to handle fact that RobotC does not have unsigned ints.
		temp_number = ( temp_number < 0 ) ? -1 * temp_number : temp_number;

		// Limit value to range.
		true_value = temp_number % range + min_value;

		return(true_value);
	}

	// Increment total number of guesses.
	total_meas++;

	if ( true_value > meas_point ) {

		// Return -1 if true value bigger than set point.
		return(-1);

		} else if ( true_value < meas_point ) {

		// Return 1 if true value less than set point
		return(1);

		} else {

		// Return 0 if measurement point matches
		correct_guess++;
		return(0);

	}
}

task main()
{
	// Run multiple trials and keep track of number of trials.
	int current_trial;

	// Storage of true value. USE ONLY FOR DEBUGGING.
	int true_value;

	// For each trial
	for ( current_trial = 1; current_trial <= total_trials ; current_trial++ ) {

		// Generate new value to measure
		true_value = resolve_measurement(-1);

		// Range to search for true value
		int min_v = min_value;
		int max_v = max_value;

		// Modify the code starting here.

		//sets a to nearest integer value

		// Estimation code here.
		// Sweep through range from lowest to highest possible value.
		// Stop when measurment_point matches true value.
		while( true ) {

			//sets current_meas_point to nearest integer value in between range
			int current_meas_point = (max_v+min_v)/2
			//calculates value of meas point depending greater/equal/lesser
		  int a = resolve_measurement(current_meas_point);

		  if(a == 1){
				// Move current_meas_point down by one, if a = 1.
				max_v = current_meas_point - 1;
			} else if (a == -1){
				// Move current_meas_point up by one, if a = -1.
				min_v = current_meas_point + 1;
			//if a = 0, breaks while loop
			}else break;
		}

		// All modified code should be above this line.

		// Write output to debug stream.
		// Keep this line for evaluation of performance.
		writeDebugStream("%d trials and %d guesses with %d correct\n",current_trial,total_meas,correct_guess);

	}

	// Wait for an infinite amount of time.
	// Keeps the Debug stream open so we can read the output of the program.
	while(1);
}
