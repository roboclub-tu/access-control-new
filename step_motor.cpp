#include "step_motor.h"
#include "appconfig.h"
#include "Arduino.h"

int minStepsToUnlock = 0;
int stepsToFullUnlock = 200; //TODO change

//Constructor
StepMotor::StepMotor() {
	pinMode(PIN_STEP, OUTPUT);           // set pin to input
	digitalWrite(PIN_STEP, LOW);       // turn on pullup resistors

	pinMode(PIN_DIR, OUTPUT);           // set pin to input
	digitalWrite(PIN_DIR, LOW);       // turn on pullup resistors
	
}

//
void StepMotor::unlock() {
  if(!digitalRead(PIN_ENDSTOP)){
  	digitalWrite(PIN_DIR, HIGH);
  	//220 steps = 1 rotation
  	for(int i = 0; i < stepsToFullUnlock; i++){
  		digitalWrite(PIN_STEP, HIGH);
  		delay(10);
  		digitalWrite(PIN_STEP, LOW);
  		delay(10);
  	}
  	digitalWrite(PIN_DIR, LOW);
    Serial.println("DOOR UNLOCKED");
	} else {
    Serial.println("LOCK NOT IN STATE TO BE UNLOCKED!!!");
	}
}


void StepMotor::lock() {
  if(!digitalRead(PIN_ENDSTOP)) {
    Serial.println("DOOR LOCKED (from checkIfLocked function)");
  } else {
    Serial.println("CRITICAL ERROR, check checkIfLocked function");
  }
}

bool StepMotor::checkIfLocked() {
	if(!digitalRead(PIN_ENDSTOP)) {
    return true;
  } else {
    short steps = stepsToStopper();
    
    if(steps > minStepsToUnlock) {
      return false; //unlocked
    } else {
      return true; // locked
    }
  }
}

//Checks how many steps are there to the stopper
short StepMotor::stepsToStopper() {
  //we need to make sure the door is closed at all times
  short count = 0;
  
  Serial.println("Testing steps to stopper...");

  while(digitalRead(PIN_MAGNET) && !digitalRead(PIN_ENDSTOP)) {
    moveToLock(1); //do 1 step at a time
    count++;
  }
  
  //if the door was opened during the testing, revert the steps for protect the door
  if(!digitalRead(PIN_MAGNET)) {
    Serial.println("DOOR OPENED!");
    moveToUnlock(count);
    return -1;
  }
  return count;
}

//Returns 0 if the door becomes locked, 1 if the door becomes unlocked
bool StepMotor::changeLockState() {
  if(checkIfLocked()){
    unlock();
  } else {
    lock();
  }
}

//Moves the lock to an unlocked state. DOESN'T CHECK FOR SENSORS!
void StepMotor::moveToUnlock(short steps) {
    digitalWrite(PIN_DIR, HIGH);
  //220 steps = 1 rotation
  for(int i = 0; i < steps; i++){
    digitalWrite(PIN_STEP, HIGH);
    delay(10);
    digitalWrite(PIN_STEP, LOW);
    delay(10);
  }
}

//Moves the lock to a locked state. DOESN'T CHECK FOR SENSORS!
void StepMotor::moveToLock(short steps) {
    digitalWrite(PIN_DIR, LOW);
  //220 steps = 1 rotation
  for(int i = 0; i < steps; i++){
    digitalWrite(PIN_STEP, HIGH);
    delay(10);
    digitalWrite(PIN_STEP, LOW);
    delay(10);
  }
}
