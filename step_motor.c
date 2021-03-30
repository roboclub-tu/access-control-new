#include <step_motor.h>

void initializeStepMotor(){
  pinMode(step_pin, OUTPUT);           // set pin to input
  digitalWrite(step_pin, LOW);       // turn on pullup resistors

  pinMode(dir_pin, OUTPUT);           // set pin to input
  digitalWrite(dir_pin, LOW);       // turn on pullup resistors

  pinMode(en_pin, OUTPUT);           // set pin to input
  digitalWrite(en_pin, HIGH);       // turn on pullup resistors

  pinMode(ms1_pin, OUTPUT);           // set pin to input
  digitalWrite(ms1_pin, LOW);       // turn on pullup resistors

  pinMode(ms2_pin, OUTPUT);           // set pin to input
  digitalWrite(ms2_pin, LOW);       // turn on pullup resistors

  pinMode(ms3_pin, OUTPUT);           // set pin to input
  digitalWrite(ms3_pin, LOW);       // turn on pullup resistors
}

bool lockState() {
  return false;
}

bool changeLockState() {
  return false;
}

void lockDoor() {
  
}

