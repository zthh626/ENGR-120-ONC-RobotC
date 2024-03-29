#pragma config(Sensor, in1,    IR1,            sensorReflection)
#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Sensor, dgtl2,  button2,        sensorTouch)
#pragma config(Sensor, dgtl3,  sonar,          sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  whiteLED,       sensorDigitalOut)
#pragma config(Sensor, dgtl6,  redLED,         sensorDigitalOut)
#pragma config(Sensor, dgtl7,  greenLED,       sensorDigitalOut)
#pragma config(Sensor, I2C_1,  linI2C,         sensorNone)
#pragma config(Motor,  port1,           rightWheel,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           leftWheel,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           linearMotor,   tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//IR Phototransistor threshold number
const int threshold = 140;
//Distance required for connection
const int connectionDistance = 7;
//Distance away from wall before reversing and turning to prevent collision
const int wallDistance = 25;

//Booleans to check for button presses
bool button1p;
bool button2p;

//Enumeratioin for turn function
enum control{Forward, Left, Right};
enum control direction;

//Value for stoppiing motor
int mstop = 0;
//Default value for motor speed
int speed = 34;
//Speed for turning
int turnSpeed = 15;

//returns true if a button is pushed and sets button1p or button2p to true
bool isPushed(){
	if(SensorValue(button1) && !button1p)
	{
		button1p = true;
		return true;
		} else if(SensorValue(button2) && !button2p){
		button2p = true;
		return true;
	}

	return false;
}

//goes backward for 1500 ms and then stops the motors
void backward(){
	motor[leftWheel] = -speed;
	motor[rightWheel] = -speed;
	wait10Msec(150);
	motor[leftWheel] = mstop;
	motor[rightWheel] = mstop;
}

//will turn left or right depending on the control enumeration named
//	direction. It turns by making one motor go one direction at turn
//	speed and the other motor the opposite direction at turn speed.
void turn(){
	if(direction == Right){
		motor[rightWheel] = -turnSpeed*2;
		motor[leftWheel] = turnSpeed*2;
		}else {
		motor[rightWheel] = turnSpeed*2;
		motor[leftWheel] = -turnSpeed*2;
	}
}

//returns true if sonar detects if the robot is less than connectionDistance
//	otherwise false
bool connectionDetect(){
	if(SensorValue[sonar] <= connectionDistance){
		return true;
	}
	return false;
}

//returns true if sonar detects if the robot is less than wallDistance
//	otherwise false
bool wallDistanceDetect(){
	if(SensorValue[sonar] <= wallDistance){
		return true;
	}
	return false;
}

//aligns robot with beacon
void align(){
	//sets enum to turn right
	direction = Right;
	//keeps turning until IR1 value is less than threshold
	while(SensorValue(IR1) > threshold){
		turn();
	}
	//resets direction to forward
	direction = Forward;
	//stops motors
	motor[leftWheel] = mstop;
	motor[rightWheel] = mstop;
}

//Makes robot go straight wheen wall is detected, reverses
//	turns right and continues going forward in an infinite loop
void wallDetect(){
	while(true){
		//makes robot go straight
		motor[leftWheel] = speed;
		motor[rightWheel] = speed;
		//checks if robot is less than wallDistance
		if(wallDistanceDetect()){
			//reverses
			backward();
			//turns right
			direction = Right;
			turn();
			//waits 1000 ms
			wait10Msec(100);
		}
	}
}

//turns LEDs on and off based on the situation
//	whiteLED turns on  when too far for connection and
//	IR1 does not break threshold
//	redLED turns on when close enough for connection
//	greenLED only turns on when close enough for connection
//	and IR1 breaks threshold
task sense(){
	while(true){
		if(connectionDetect() && SensorValue[IR1] < threshold){
			SensorValue[whiteLED] = 0;
			SensorValue[redLED] = 1;
			SensorValue[greenLED] = 1;
			} else if(connectionDetect()){
			SensorValue[whiteLED] = 0;
			SensorValue[redLED] = 1;
			SensorValue[greenLED] = 0;
			} else {
			SensorValue[whiteLED] = 1;
			SensorValue[redLED] = 0;
			SensorValue[greenLED] = 0;
		}
	}
}

task main(){
	//starts task that turns on LEDs based on situation
	startTask(sense);
	//sets direction to forwad
	direction = Forward;

	//starts while loop that looks for button presses
	while(true){
		//checks for button push
		if(isPushed()){
			if(button1p == true){
				//starts wall detect
				wallDetect();
				button1p = button2p = false;
			}
			else if(button2p == true){
				//aligns robot
				align();
				button1p = button2p = false;
			}
		}
	}
}
