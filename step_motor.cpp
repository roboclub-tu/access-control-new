#include "step_motor.h"
#include "appconfig.h"
#include "Arduino.h"

//TODO move to appconfig.h
//int minStepsToUnlock = 750;
//int stepsToFullUnlock = 1050; //TODO test
//int stepsBeforeLockDown = 2500;



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
  	moveToUnlock(STEPS_FULL_TO_UNLOCK);
    Serial.println("DOOR UNLOCKED");
	} else {
    Serial.println("DOOR NOT IN STATE TO BE UNLOCKED!!!");
	}
}

//Since checkIfLocked() function tests steps to endstops, once its reached, door is practically locked
void StepMotor::lock() {
  if(!digitalRead(PIN_ENDSTOP)) {
    Serial.println("DOOR LOCKED (from checkIfLocked function)");
  } else {
    Serial.println("CRITICAL ERROR, check checkIfLocked function");
  }
}

//Uses stepsToStopper to see how many steps the motor has made until reaching endstop
//after this function the door will be LOCKED
//returns:
//    true: endstop is already pressed OR steps to endstop were not enough for door to be unlocked (door WAS locked)
//    false: steps to endstop WERE enough for door to be unlocked (door WAS unlocked)
//
//return statement is used to determine if door should stay locked or it should unlock  
bool StepMotor::checkIfLocked() {
	if(digitalRead(PIN_ENDSTOP)) {
    if(stepsToStopper() > STEPS_MIN_TO_UNLOCK) {
      Serial.println("Steps to stopper > minsteps to unlock -> UNLOCKED");
      return false; //unlocked
    } else {
      Serial.println("Steps to stopper < minsteps to unlock -> LOCKED");
      return true; // locked
    }
  } else { 
    Serial.println("Stopper pressed -> LOCKED");   
    return true; //locked 
  }
}

//Checks how many steps are there to the endstop sensor
short StepMotor::stepsToStopper() {
  short count = 0; //keep count of steps made so far
  
  Serial.println("Testing steps to stopper...");

  digitalWrite(PIN_EN, LOW); //turn on the driver
  digitalWrite(PIN_DIR, LOW); //set direction

  //Door must be closed, endstop not reached, count of steps less than the maximum the lock can actually rotate
  while(digitalRead(PIN_MAGNET) && digitalRead(PIN_ENDSTOP) && count <= STEPS_BEFORE_LOCKDOWN) {
    digitalWrite(PIN_STEP, HIGH);
    delay(2);
    digitalWrite(PIN_STEP, LOW);
    delay(2);
    count++;
  }
  
  //if the door was opened during the testing, revert the steps for protect the door
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
event StepMotor::changeLockState() {
  //TODO check if error state is needed (open door, pressed endstop)
  if(!digitalRead(PIN_MAGNET)){ 
    return SCAN;
  } else if(checkIfLocked()) {
    unlock();
    return UNLOCK;
  } else {
    return LOCK;
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
