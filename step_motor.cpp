#include "step_motor.h"
#include "appconfig.h"
#include "Arduino.h"

//TODO move to appconfig.h
int minStepsToUnlock = 750;
int stepsToFullUnlock = 1050; //TODO test
int stepsBeforeLockDown = 2500;

//Constructor
StepMotor::StepMotor() {
	pinMode(PIN_STEP, OUTPUT);           // set pin to input
	digitalWrite(PIN_STEP, LOW);       // turn on pullup resistors

	pinMode(PIN_DIR, OUTPUT);           // set pin to input
	digitalWrite(PIN_DIR, LOW);       // turn on pullup resistors

  pinMode(PIN_EN, OUTPUT);
  digitalWrite(PIN_EN, HIGH);
}

//Doesn't check if door is closed, may cause an error if endstop is broken
void StepMotor::unlock() {
  if(!digitalRead(PIN_ENDSTOP)){
  	moveToUnlock(stepsToFullUnlock);
    Serial.println("DOOR UNLOCKED");
	} else {
    Serial.println("DOOR NOT IN STATE TO BE UNLOCKED!!!");
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
	if(digitalRead(PIN_ENDSTOP)) { //TODO could be simplified since steps can be 0
    if(stepsToStopper() > minStepsToUnlock) {
      return false; //unlocked
    } else {
      return true; // locked
    }
  } else {    
    return true; //locked 
  }
}

//Checks how many steps are there to the stopper
short StepMotor::stepsToStopper() {
  short count = 0; //keep count of steps made so far
  
  Serial.println("Testing steps to stopper...");

  digitalWrite(PIN_EN, LOW); //turn on the driver
  digitalWrite(PIN_DIR, LOW); //set direction

  //Door must be closed, endstop not reached, count of steps less than the maximum the lock can actually rotate
  while(digitalRead(PIN_MAGNET) && !digitalRead(PIN_ENDSTOP) && count <= stepsBeforeLockDown) {
    digitalWrite(PIN_STEP, HIGH);
    delay(2);
    digitalWrite(PIN_STEP, LOW);
    delay(2);
    count++;
  }
  
  //if the door was opened during the testing, revert the steps for protect the door
  //TODO implement debouncing, often one reading is false, then true
  //FIXME rewrite so it reverts with only one FALSE reading
  if(!digitalRead(PIN_MAGNET)) {
    Serial.println("DOOR OPENED!");
    moveToUnlock(count); //revert progress to avoid damage to the door
    return -1;
  }

  digitalWrite(PIN_EN, HIGH); //disable driver
  
  return count;
}

//In the checkIfLocked method, lock goes to locked state to reach endstop.
void StepMotor::changeLockState() {
  if(checkIfLocked()){
    unlock();
  }
}

//Moves the lock to an unlocked state. DOESN'T CHECK FOR SENSORS!
void StepMotor::moveToUnlock(short steps) {
  
  digitalWrite(PIN_EN, LOW);
  digitalWrite(PIN_DIR, HIGH);
  
  for(int i = 0; i < steps; i++){
    digitalWrite(PIN_STEP, HIGH);
    delay(2);
    digitalWrite(PIN_STEP, LOW);
    delay(2);
  }

  digitalWrite(PIN_EN, HIGH);
}
