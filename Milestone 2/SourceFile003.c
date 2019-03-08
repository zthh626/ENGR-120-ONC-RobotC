#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Sensor, dgtl2,  button2,        sensorTouch)
#pragma config(Sensor, dgtl3,  switchLinear,   sensorTouch)
#pragma config(Sensor, I2C_1,  motorI2C,       sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           motorBackRight, tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           motorSteer,    tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           motorLinear,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          motorBackLeft, tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

bool button1p;
bool button2p;
bool switcher;

enum turn {Forward , Left, Right};
enum turn direction;

enum orientation {Down, Up};
enum orientation linear;

int speed = 25;
int steerSpeed = 20;
int turnSpeed = 28;
int motorStop = 0;
int turnValue = 120;

bool isPushed(){
	if(SensorValue(button1) && !button1p)
	{
		button1p = true;
		return true;
	}

	if(SensorValue(button2) && !button2p)
	{
		button2p = true;
		return true;
	}

	if(SensorValue(switchLinear) && !switcher){
		switcher = true;
		return true;
	}

	return false;
}

void straightOne(){
	int count = 0;
	while(count < 5){
		button1p = false;
		button2p = false;
		motor[motorBackLeft] = speed;
		motor[motorBackRight] = speed;

		count++;
		wait1Msec(1000);
	}
}

void turner(){

	//turns steer direction
	if(direction == Right){
		while(SensorValue[motorI2C] <= turnValue){
			motor[motorSteer] = steerSpeed;
		}
		} else {
		while(SensorValue[motorI2C] >= -turnValue){
			motor[motorSteer] = -steerSpeed;
		}
	}
	//stops steer motor
	motor[motorSteer] = motorStop;

	//slows one wheel down and turns in direction
	if(direction == Right){
		motor[motorBackLeft] = turnSpeed;
		motor[motorBackRight] = -turnSpeed;
		}else{
		motor[motorBackLeft] = -turnSpeed;
		motor[motorBackRight] = turnSpeed;
	}
	wait10Msec(133);
	motor[motorBackLeft] = motorStop;
	motor[motorBackRight] = motorStop;

	//puts steering wheel back
	if(direction == Right){
		while(SensorValue[motorI2C] >= 0){
			motor[motorSteer] = -steerSpeed;
		}
		} else {
		while(SensorValue[motorI2C] <= 0){
			motor[motorSteer] = steerSpeed;
		}
	}

	motor[motorSteer] = motorStop;
}

void linearMotion(){
	if(linear == Up){
		motor[motorLinear] = 17;
		wait10Msec(150);
		motor[motorLinear] = 0;
		linear = Down;
		} else {
		motor[motorLinear] = -17;
		wait10Msec(150);
		motor[motorLinear] = 0;
		linear = Up;
	}
}

void disconnect(){
	linearMotion();
	motor[motorBackLeft] = -speed;
	motor[motorBackRight] = -speed;
	direction = Left;
	wait10Msec(150);
	motor[motorBackLeft] = 0;
	motor[motorBackRight] = 0;
	//turns steer direction
	if(direction == Right){
		while(SensorValue[motorI2C] <= turnValue){
			motor[motorSteer] = steerSpeed;
		}
		} else {
		while(SensorValue[motorI2C] >= -turnValue){
			motor[motorSteer] = -steerSpeed;
		}
	}
	//stops steer motor
	motor[motorSteer] = motorStop;

	//slows one wheel down and turns in direction
	if(direction == Right){
		motor[motorBackLeft] = turnSpeed;
		motor[motorBackRight] = -turnSpeed;
		}else{
		motor[motorBackLeft] = -turnSpeed;
		motor[motorBackRight] = turnSpeed;
	}
	wait10Msec(133);
	motor[motorBackLeft] = motorStop;
	motor[motorBackRight] = motorStop;
	motor[motorBackLeft] = speed;
	motor[motorBackRight] = speed;
	wait10Msec(200);
	motor[motorBackLeft] = 0;
	motor[motorBackRight] = 0;
	linearMotion();
}

void test(){
	int count = 0;
	while(count < 5){
		button1p = false;
		button2p = false;
		motor[motorBackLeft] = speed;
		motor[motorBackRight] = speed;

		count++;
		wait1Msec(300);
	}
	motor[motorBackLeft] = 0;
	motor[motorBackRight] = 0;
	wait1Msec(2000);
	disconnect();
}

task main(){
	button1p = button2p = false;
	direction = Forward;

	SensorValue[motorI2C] = 0;

	while(true){
		if(isPushed()){
			if(button1p == true){
				straightOne();
				motor[motorBackLeft] = motorStop;
				motor[motorBackRight] = motorStop;
				button1p = button2p = switcher = false;
				}else if(button2p == true){
				direction = Right;
				turner();
				direction = Forward;
				button1p = button2p = switcher = false;
				} else {
				test();
				//linearMotion();
				button1p = button2p = switcher = false;
			}
		}
	}
}
