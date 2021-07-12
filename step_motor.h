#ifndef STEP_MOTOR_H_
#define STEP_MOTOR_H_

#include "Arduino.h"
#include "appconfig.h"

//This file has the purpose of simplifying the main file, by automating everything

class StepMotor 
{
	private:
		void unlock();
		void lock();
		bool isLocked; 
    short stepsToStopper();
    void moveToLock(short steps);
    void moveToUnlock(short steps);
    bool checkIfLocked();
	
	public:
		StepMotor();
		
    //If door is locked, unlocks it and vise versa
		event changeLockState();
};

#endif // STEP_MOTOR_H_
