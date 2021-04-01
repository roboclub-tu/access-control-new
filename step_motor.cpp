#include "step_motor.h"
#include "appconfig.h"
#include "Arduino.h"

//Constructor
StepMotor::StepMotor() {
	pinMode(PIN_STEP, OUTPUT);           // set pin to input
	digitalWrite(PIN_STEP, LOW);       // turn on pullup resistors

	pinMode(PIN_DIR, OUTPUT);           // set pin to input
	digitalWrite(PIN_DIR, LOW);       // turn on pullup resistors
}

void StepMotor::Unlock() {
	//TODO stepper motor functionality
}


void StepMotor::Lock() {
	//TODO stepper motor functionality
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
