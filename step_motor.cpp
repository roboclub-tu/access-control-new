#include "step_motor.h"
#include "appconfig.h"
#include "Arduino.h"

//Constructor
StepMotor::StepMotor() {
	pinMode(PIN_STEP, OUTPUT);           // set pin to input
	digitalWrite(PIN_STEP, LOW);       // turn on pullup resistors

	pinMode(PIN_DIR, OUTPUT);           // set pin to input
	digitalWrite(PIN_DIR, LOW);       // turn on pullup resistors
	
	state = false;
}

void StepMotor::Unlock() {
	//TODO stepper motor functionality
	digitalWrite(PIN_DIR, HIGH);
	//220 steps = 1 rotation
	for(int i = 0; i < STEP_PER_REV; i++){
		digitalWrite(PIN_STEP, HIGH);
		delay(10);
		digitalWrite(PIN_STEP, LOW);
		delay(10);
	}
	digitalWrite(PIN_DIR, LOW);
	state = !state;
}


void StepMotor::Lock() {
	digitalWrite(PIN_DIR, LOW);
	//220 steps = 1 rotation
	for(int i = 0; i < STEP_PER_REV; i++){
		digitalWrite(PIN_STEP, HIGH);
		delay(10);
		digitalWrite(PIN_STEP, LOW);
		delay(10);
	}
	state = !state;
}

bool StepMotor::CheckState() {
	return 0;
}

//Returns 0 if the door becomes locked, 1 if the door becomes unlocked
bool StepMotor::ChangeLockState() {
	if(CheckState()){
		Lock();
		return 0;
	} else {
		Unlock();
		return 1;
	}
}
