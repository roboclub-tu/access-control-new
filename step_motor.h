
#ifndef STEP_MOTOR_H_
#define STEP_MOTOR_H_

//This file has the purpose of simplifying the main file, by automating everything

void initializeStepMotor();
bool lockState();
bool changeLockState();
void lockDoor();

#endif // STEP_MOTOR_H_