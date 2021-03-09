/*

//This file has the purpose of simplifying the main file, by automating everything

//Transfering variables from the main tab
extern const int step_pin;
extern const int dir_pin;
extern const int en_pin;
extern const int ms1_pin;
extern const int ms2_pin;
extern const int ms3_pin;

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

*/
