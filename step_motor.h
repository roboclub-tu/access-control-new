#ifndef STEP_MOTOR_H_
#define STEP_MOTOR_H_

#include "Arduino.h"

//This file has the purpose of simplifying the main file, by automating everything

class StepMotor 
{
	private:
		void Unlock();
		void Lock();
		bool isLocked; 
    short StepsToStopper();
    void MoveToLock(short steps);
    void MoveToUnlock(short steps);
	
	public:
		StepMotor();
		
		//Returns 0 if the door is unlocked, 1 if the door is locked
		bool CheckIfLocked();
		
		//Returns 0 if the door becomes locked, 1 if the door becomes unlocked
		bool ChangeLockState();
};

#endif // STEP_MOTOR_H_
