#include "step_motor.h"
#include "appconfig.h"
#include "Arduino.h"

int minStepsToUnlock = 0;

//Constructor
StepMotor::StepMotor() {
	pinMode(PIN_STEP, OUTPUT);           // set pin to input
	digitalWrite(PIN_STEP, LOW);       // turn on pullup resistors

	pinMode(PIN_DIR, OUTPUT);           // set pin to input
	digitalWrite(PIN_DIR, LOW);       // turn on pullup resistors
	
	isLocked = false;
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
	isLocked = !isLocked;
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
	isLocked = !isLocked;
}

bool StepMotor::CheckIfLocked() {
	if(!digitalRead(PIN_ENDSTOP)) {
    return true;
  } else {
    short steps = StepsToStopper();
    
    if(steps > minStepsToUnlock) {
      return false; //unlocked
    } else {
      return true; // locked
    }
  }
}

//Checks how many steps are there to the stopper and reverts it, then returns the number
short StepMotor::StepsToStopper() {
  //we need to make sure the door is closed at all times
  short count = 0;
  
  Serial.println("Testing steps to stopper...");

  while(digitalRead(PIN_MAGNET) && !digitalRead(PIN_ENDSTOP)) {
    digitalWrite(PIN_DIR, HIGH); //TODO change after testing

    digitalWrite(PIN_STEP, HIGH);
    delay(5);
    digitalWrite(PIN_STEP, LOW);
    delay(5);
    count++;
  }
  
  //readings are fast so it shouldn't change
  if(!digitalRead(PIN_MAGNET)) {
    Serial.println("DOOR OPENED!");
  }

  //reverting lock to previous state
  for(int i = count; i > 0; i--) {
    digitalWrite(PIN_DIR, LOW); //TODO change after testing

    digitalWrite(PIN_STEP, HIGH);
    delay(5);
    digitalWrite(PIN_STEP, LOW);
    delay(5);
  }

  return count;

}

//Returns 0 if the door becomes locked, 1 if the door becomes unlocked
bool StepMotor::ChangeLockState() {
  return false;
}

void StepMotor::MoveToUnlock(short steps) {

}

void StepMotor::MoveToLock(short steps) {
  
}
