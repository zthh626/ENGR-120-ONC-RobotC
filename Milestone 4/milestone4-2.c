#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    iFLeft,         sensorReflection)
#pragma config(Sensor, in2,    iFRight,        sensorReflection)
#pragma config(Sensor, dgtl6,  ir,             sensorDigitalOut)
#pragma config(Sensor, dgtl8,  button2,        sensorTouch)
#pragma config(Sensor, dgtl9,  button1,        sensorTouch)
#pragma config(Sensor, dgtl11, sonar,          sensorSONAR_cm)
#pragma config(Sensor, I2C_1,  linI2C,         sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           right,         tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           left,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           linear,        tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int correctDistance = 40;
int delta;
//values of IR sensors that get updated periodically
int vFLeft;
int vFRight;

//booleans for if button1 and button2 if they get pushed
bool button1p;
bool button2p;

//enumeration for directions to turn
enum control{Forward, Left, Right};
enum control direction;

//enumeration for linear motion mechanism to move up or down
enum orientation {Down, Up};
enum orientation linearO;

//distance for wallDetect(), when distance gets less than this number
//	will return false in function
int distanceConnect = 10;
int distanceWall = 20;

//minlevel for IR sensors in monitorFunction(),
int minLevelF = 600;

//threshold for IR sensors
int frontThreshold = 200;

//values for motor
//stoping value
int mstop = 0;
//linear motion mechanism motor speed
int linSpeed = 15;
//default speed for moving forward
int speed = 35;
//turning speed
int turnSpeed  = 25;

//if linearO is UP or Down will move mechanism up or down
//	should go down first and then go up
void linearMotion(){
	if(linearO == Up){
		//moves motor at linSpeed
		motor[linear] = linSpeed;
		//waits until I2C value is more than 20
		waitUntil(SensorValue[linI2C] <= -200);
		//stops motor
		motor[linear] = mstop;
		//sets to opposite direction
		linearO = Down;
		} else {
		//moves motor at opposite speed
		motor[linear] = -linSpeed;
		//waits until I2C is less than -100
		waitUntil(SensorValue[linI2C] >= 0);
		//stop motor
		motor[linear] = mstop;
		//sets to opposite direction
		linearO = Up;
	}
}

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

//updates values of each ir sensor every pulse of the beacon detected
void monitorLight(){
	//min, max, dif level for iFLeft sensor
	static int minLevelIR1 = minLevelF;
	static int maxLevelIR1 = 0;
	static int diffLevelIR1 = 0;

	//min, max, dif level for iFRight sensor
	static int minLevelIR2 = minLevelF;
	static int maxLevelIR2 = 0;
	static int diffLevelIR2 = 0;

	//gets value for each sensor
	int lightLevel1 = SensorValue[iFLeft];
	int lightLevel2 = SensorValue[iFRight];

	//determines value for sensor each every 100 ms
	if ( time1[T1] > 100 )  {
		//determines difflevel for each sensor, sets maxlevel to 0,
		//	and minlevel to default level
		diffLevelIR1 = maxLevelIR1 - minLevelIR1;
		maxLevelIR1 = 0;
		minLevelIR1 = minLevelF;

		diffLevelIR2 = maxLevelIR2 - minLevelIR2;
		maxLevelIR2 = 0;
		minLevelIR2 = minLevelF;

		//clears timer
		clearTimer(T1);
		} else {
		//if lightlevel of each sensor is less than minlevel
		//	then minlevel will equal lightlevel
		if ( lightLevel1 < minLevelIR1 ) {
			minLevelIR1 = lightLevel1;
			//and then if lightlevel is greater than maxlevel
			//	maxlevel will  equal lightlevel
			} else if ( lightLevel1 > maxLevelIR1 ) {
			maxLevelIR1 = lightLevel1;
		}
		if ( lightLevel2 < minLevelIR2 ) {
			minLevelIR2 = lightLevel2;
			} else if ( lightLevel2 > maxLevelIR2 ) {
			maxLevelIR2 = lightLevel2;
		}
	}

	//updates each value
	vFLeft = diffLevelIR1;
	vFRight = diffLevelIR2;
}

//turns left or right
//	direction is determined before function is called
//	turns by rotating one wheel one direction and the other the
//	opposite ways
void turn(){
	if(direction == Right){
		//-10 for warped gear
		motor[right] = -turnSpeed -10;
		motor[left] = turnSpeed;
		}else if(direction == Left){
		motor[right] = turnSpeed;
		motor[left] = -turnSpeed;
	}
}

//returns true if robot is less than distanceConnect
//	false otherwise
bool detect(){
	if(SensorValue[sonar] <= distanceConnect){
		return true;
	}
	return false;
}

bool detectWall(){
	if(SensorValue[sonar] <= distanceWall){
		return true;
	}
	return false;
}

//determines direction and returns true if only sensors in the front
//	break threshold and returns false for all cases
bool determineTurn(){

	//if only front sensors break threshold, direction = forward and returns true
	//vFRight > frontThreshold && vFLeft > frontThreshold
	if(vFRight > frontThreshold && vFLeft > frontThreshold){
		if(SensorValue[sonar] < correctDistance){
			delta = 30;
		}else {
			delta = 100;
		}
		if(abs(vFRight - vFLeft) < delta) direction = Forward;
		else if(vFRight - vFLeft < 0) direction = Left;
		else if(vFRight - vFLeft > 0) direction = Right;
		else direction = Forward;
		return true;
		//if vRight or vLeft breaks threshold
	}
	direction = Right;

	//for every case except front sensors break threshold it returns false
	return false;
}

bool wallDetection(){
	if(detectWall() && !determineTurn()){
		motor[left] = -speed;
		motor[right] = -speed;
		wait10Msec(100);
		direction = Right;
		turn();
		wait10Msec(200);
		motor[left] = mstop;
		motor[right] = mstop;
		direction = Forward;
		return true;
	}
	return false;
}

//will be called at button press, will make connection.
//	a while loop will run until determineTurn() returns true, that's when
//	both front sensors are facing beacon, and detect() returns true.
//	detect() returns true when it has reached the appropriate connection distance.
//	if the direction is forward but detect() is still false, the robot will go straight.
void changeDirection(){
	//(determineTurn() && !detect()) || (!determineTurn() && !detect())
	//&& (vFLeft < (frontThreshold + 300) && vFRight < (frontThreshold + 300))
	while(!detect()){
		determineTurn();
		if(wallDetection()) continue;
		turn();
		if(direction == Forward){
			motor[right] = speed;
			motor[left] = speed;
		}
		//wait10Msec(30);
	}
	motor[left] = mstop;
	motor[right] = mstop;
}

//called after changeDirection(), series of steps that will disconnect wire from robot
void disconnect(){
	//lowers linearmotion mechanism
	linearMotion();
	//reverses robot for a certain amount of time
	motor[left] = -speed;
	motor[right] = -speed;
	wait10Msec(100);
	//sets direction to turn right and turns
	direction = Left;
	turn();
	//waits and then stops motors
	wait10Msec(300);
	direction = Forward;
	while(!wallDetection()){
		motor[left] = speed;
		motor[right] = speed;
		if(time1[T2] == 200){
			clearTimer(T2);
			break;
		}
	}
	//waits and then stops motor
	motor[left] = mstop;
	motor[right] = mstop;
	linearMotion();
}

task ledHz(){
	while(true){
		wait10Msec(10);
		SensorValue[ir] = 1;
		wait10Msec(10);
		SensorValue[ir] = 0;
	}
}

//calls the monitorLight() function
task updateIRValues(){
	while(true){
		monitorLight();
	}
}

task main(){

	startTask(ledHz);

	//starts task that up updates IR values
	startTask(updateIRValues);

	//initializes enum values and I2c
	direction = Forward;
	linearO = Up;
	SensorValue[linI2C] = 0;

	while(true){
		if(isPushed()){
			if(button1p == true){
				changeDirection();
				//linearMotion();

				/*
				motor[left] = speed;
				motor[right] = speed;
				wait10Msec(100);
				motor[left] = mstop;
				motor[right] = mstop;
				*/
				disconnect();


				button1p = button2p = false;
			}
			else if(button2p == true){
				//motor[linear] = -linSpeed;
				linearMotion();

				button1p = button2p = false;
			}
		}
	}
}