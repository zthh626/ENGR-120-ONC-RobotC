#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    front1i,        sensorReflection)
#pragma config(Sensor, in2,    front2i,        sensorReflection)
#pragma config(Sensor, in3,    righti,         sensorNone)
#pragma config(Sensor, in4,    backi,          sensorReflection)
#pragma config(Sensor, in5,    lefti,          sensorNone)
#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Sensor, dgtl2,  button2,        sensorTouch)
#pragma config(Sensor, dgtl3,  sonar,          sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  led1,           sensorDigitalOut)
#pragma config(Sensor, dgtl9,  led2,           sensorDigitalOut)
#pragma config(Sensor, dgtl12, led3,           sensorDigitalOut)
#pragma config(Sensor, I2C_1,  linI2C,         sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           right,         tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           left,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           linear,        tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const int threshold = 140;
const int meter = 7;
const int wallMeter = 25;

bool button1p;
bool button2p;

enum control{Forward, Left, Right};
enum control direction;

enum orientation {Down, Up};
enum orientation linearO;

int mstop = 0;
int gospeed = 34;
int speed = 15;
int linSpeed = 15;

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

void backward(){
	motor[left] = -gospeed;
	motor[right] = -gospeed;
	wait10Msec(150);
	motor[left] = mstop;
	motor[right] = mstop;
}

void turn(){
	if(direction == Right){
		motor[right] = -speed*2;
		motor[left] = speed*2;
		}else {
		motor[right] = speed*2;
		motor[left] = -speed*2;
	}
}

void linearMotion(){
	if(linearO == Up){
		motor[linear] = linSpeed;
		waitUntil(SensorValue[linI2C] >= 20);
		motor[linear] = mstop;
		linearO = Down;
		} else {
		motor[linear] = -linSpeed;
		waitUntil(SensorValue[linI2C] <= -100);
		motor[linear] = mstop;
		linearO = Up;
	}
}

bool detect(int num){
	if(num == 1){
		if(SensorValue[sonar] <= meter){
			return true;
		}
	} else if(num == 2){
		if(SensorValue[sonar] <= wallMeter){
			return true;
		}
	}
	return false;
}


void correct(){
	direction = Right;
	while(SensorValue(front1i) > threshold){
		turn();
	}
	motor[left] = mstop;
	motor[right] = mstop;
}

void mile2(){
	direction = Forward;
	correct();
	while(!detect(1)){
		direction = Forward;
		motor[left] = gospeed;
		motor[right] = gospeed;
	}
	motor[left] = mstop;
	motor[right] = mstop;
}

void wallDetect(){
	while(){
		motor[left] = gospeed;
		motor[right] = gospeed;
		if(detect(2)){
			backward();
			direction = right;
			turn();
			wait10Msec(100);
		}
	}
}

task sense(){
	while(true){
		if(detect(1) && SensorValue[front1i] < threshold){
			SensorValue[led1] = 0;
			SensorValue[led2] = 1;
			SensorValue[led3] = 1;
		} else if(detect(1)){
			SensorValue[led1] = 0;
			SensorValue[led2] = 1;
			SensorValue[led3] = 0;
		} else {
			SensorValue[led1] = 1;
			SensorValue[led2] = 0;
			SensorValue[led3] = 0;
		}
	}
}

task main(){
	startTask(sense);
	direction = Forward;
	linearO = Down;
	SensorValue[linI2C] = 0;

	while(true){
		if(isPushed()){
			if(button1p == true){
				//test();
				//correct();
				wallDetect();
				button1p = button2p = false;
			}
			else if(button2p == true){
				correct();
				//linearMotion();
				button1p = button2p = false;
			}
		}
	}
}
