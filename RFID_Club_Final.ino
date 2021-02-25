/////////////////////////// USED HARDWARE - READ CAREFULY! ///////////////////////////

//IMPORTANT - SELECT ONLY ONE (STEPPER MOTOR / MAGNETIC LOCK)
  //Functionality not yet added

  //Uncheck this to activate stepper motor code
  //#define USING_STEP_MOTOR

  //Uncheck this to activate electromagnetic lock code
  //#define USING_MAGNETIC_LOCK
//ENDIF

//IMPORTANT - SELECT ONLY ONE (WIEGAND / RFID 522)
  //use Wiegand 
  #define USING_WIEGAND
  
  //use RFID 522
  //FUNCTIONALITY NOT YET ADDED
//ENDIF

//Check if using ESP (ESP needs extra code to write to EEPROM)
#define USING_ESP

///////////////////////// DATABASE - READ CAREFULY! ////////////////////////

//ESP's emulate EEPROM  y dedicting 1 sector (4kB) of the flash memory
//Arduino Mega has 4kB of EEPROM
//Arduino Uno has 1kB of EEPROM
//Arduino Nano has 512B of EEPROM
//To calculate the size in bytes -> 4*NUM_OF_TAGS + 2
//For 64 tags, the DB uses 258 bytes of EEPROM
//To ensure compatibility, use a maximum of 127 tags

#define NUM_OF_TAGS 64

//TODO for future uses, the Database can store names, as well as the tag, in order to see who came at what time.
//TODO to calculate DB size -> (4+DB_CHAR_COUNT)*NUM_OF_TAGS + 2
//TODO It may be better for the DB to be external, but it's an option

//TODO #define DB_CHAR_COUNT 8

///////////////////////// ADDING/DELETING TAGS - READ CAREFULY! ///////////////////

//For now, only buttons have functionality
//Changing over WIFI poses security risks!

//IMPORTANT - SELECT ONLY ONE (BUTTONS / WIFI)

#define USING_BUTTONS
//#define USING_WIFI

/////////////////////////// PIN SETUP ///////////////////////

//pins connected to the card reader D0 and D1 signals.
//If using Wiegand reader, ensure your board supports external Interruptions on these pins
#define PIN_D0 32 // black 
#define PIN_D1 33 // red

// Pins connected to LED lighting of the RFID controller (not mandatory)
#define PIN_GRN_LED 25
//#define PIN_RED_LED 5 //WIEGAND READER DOESN'T HAVE RED LED CONTROL!

// Pin connected to the buzzer
#define PIN_BUZZER 26

//Buttons that when pressed, add or remove a tag
//ONLY NEEDED IF USING BUTTONS TO ADD TAGS
//READ "ADDING/DELETING TAGS" FOR MORE INFO
#ifdef USING_BUTTONS
  #define PIN_ADD_TAG 6
  #define PIN_DEL_TAG 7
#endif

//Pin setup for the step motor
//ONLY NEEDED IF USING STEP-MOTOR
//TODO bandage fix for transfering values over different tabs, change to something more elegant
#ifdef USING_STEP_MOTOR
  extern const int step_pin = 24
  extern const int dir_pin = 22
  extern const int en_pin = 28
  extern const int ms1_pin = 23
  extern const int ms2_pin = 25
  extern const int ms3_pin = 27
#endif

////////////////////////// NON REDACTABLE CODE /////////////////////////////

//Prototyping
bool checkIfStepAndLockCorrectlyDefined();
bool checkIfTagReaderCorrectlyDefined();
bool checkIfButtonsAndWifiCorrectlyDefined();
void accessGiven();
void accessDenied();


#ifdef USING_WIEGAND
  #include <Wiegand.h>
  Wiegand wiegand;
#endif

#include <RfidDb.h>

//Database storing all the tags
RfidDb database = RfidDb(NUM_OF_TAGS, 0);

void setup() {
  Serial.begin(115200);

  if(checkIfStepAndLockCorrectlyDefined() && checkIfTagReaderCorrectlyDefined() && checkIfButtonsAndWifiCorrectlyDefined()) {
    //If using ESP, we have to initialize the EEPROM
    #ifdef USING_ESP
      EEPROM.begin(database.dbSize());
    #endif

    database.begin();

    //If using buttons to add tags, init
    #ifdef USING_BUTTONS
      pinMode(PIN_ADD_TAG, INPUT);
      pinMode(PIN_DEL_TAG, INPUT);
    #endif
  
    //configuring Wiegand tag reader
    #ifdef USING_WIEGAND
      //Install listeners and initialize Wiegand reader
      wiegand.onReceive(receivedData, "Card read: ");
      wiegand.onReceiveError(receivedDataError, "Card read error: ");
      wiegand.onStateChange(stateChanged, "State changed: ");
      wiegand.begin(Wiegand::LENGTH_ANY, true);
    
      //initialize pins as INPUT and attaches interruptions
      pinMode(PIN_D0, INPUT);
      pinMode(PIN_D1, INPUT);
      attachInterrupt(digitalPinToInterrupt(PIN_D0), pinStateChanged, CHANGE);
      attachInterrupt(digitalPinToInterrupt(PIN_D1), pinStateChanged, CHANGE);
    
      //Sends the initial pin state to the Wiegand library
      pinStateChanged();

      Serial.println("Successful configure");
    #endif

    //TODO configuring RFID 522 tag reader
  } else {
    Serial.println("CRITICAL ERROR!");
    Serial.println("PROGRAM NOT CORRECTLY SET UP");
    Serial.println("CHECK YOUR CONFIGURATION!");
    exit(0);
  }
}

// Every few milliseconds, check for pending messages on the wiegand reader
// This executes with interruptions disabled, since the Wiegand library is not thread-safe
void loop() {
  noInterrupts();
  wiegand.flush();
  interrupts();
  //Sleep a little -- this doesn't have to run very often.
  delay(100);
}

// Notifies when a card was read.
void receivedData(uint8_t* data, uint8_t bits, const char* message) {
    Serial.print(message);
    Serial.print(bits);
    Serial.print("bits / ");
    //Print value in HEX
    uint8_t bytes = (bits+7)/8;
    for (int i=0; i<bytes; i++) {
        Serial.print(data[i] >> 4, 16);
        Serial.print(data[i] & 0xF, 16);
    }
    Serial.println();

    #ifdef USING_BUTTONS
      if(digitalRead(PIN_ADD_TAG) == HIGH) {
        //Returns if write was successful
        if(database.insert(data) { //TODO Debug
          Serial.println("Insert successful"); 
        } else {
          Serial.println("ERROR: Couldn't insert tag");
        }
      } else if(digitalRead(PIN_DEL_TAG) == HIGH) {
        database.remove(data); //TODO Debug
        Serial.println("Tag deleted, if there was a tag");
      } else {
        if(database.contains(data)){ //TODO Debug
          accessGiven();
        } else {
          accessDenied();
        }
      }
    #endif

    //TODO open/close action
    //TODO check for pressed button and add/del tag
}

//If the scanned tag is in the database
void accessGiven() {
  Serial.println("change state");
}

//if the scanned tag isn't in the database
void accessDenied() {
  Serial.println("haha, door go brrr");
}

/////////////////////////// DEBUG FUNCTIONS /////////////////////////

//If user defines both Step motor AND electromagnetic lock, return false
//If user didn't define any, return true (Presuming user wants to run in debug mode)
//If user defined only 1, return true
bool checkIfStepAndLockCorrectlyDefined(){
  int checker = 0;
  
  #ifdef USING_STEP_MOTOR
    checker++;
  #endif

  #ifdef USING_MAGNETIC_LOCK
    checker++;
  #endif

  switch(checker){
    case 0:
      return true;
    case 1:
      return true;
    case 2:
      return false;
    default:
      return false;
  }
}

//If user defines both Wiegand AND RFID522, return false
//If user didn't define any, return false
//If user defined only 1, return true
bool checkIfTagReaderCorrectlyDefined(){
  int checker = 0;
  
  #ifdef USING_WIEGAND
    checker++;
  #endif

  #ifdef USING_RFID522
    checker++;
  #endif

  switch(checker){
    case 0:
      return false;
    case 1:
      return true;
    case 2:
      return false;
    default:
      return false;
  }
}

//If user defines both buttons AND wifi, return false
//If user didn't define any, return false
//If user defined only 1, return true
bool checkIfButtonsAndWifiCorrectlyDefined(){
  int checker = 0;
  
  #ifdef USING_BUTTONS
    checker++;
  #endif

  #ifdef USING_WIFI
    checker++;
  #endif

  switch(checker){
    case 0:
      return false;
    case 1:
      return true;
    case 2:
      return false;
    default:
      return false;
  }
}

// When any of the pins have changed, update the state of the wiegand library
void pinStateChanged() {
  wiegand.setPin0State(digitalRead(PIN_D0));
  wiegand.setPin1State(digitalRead(PIN_D1));
}

// Notifies when a reader has been connected or disconnected.
// Instead of a message, the seconds parameter can be anything you want -- Whatever you specify on `wiegand.onStateChange()`
void stateChanged(bool plugged, const char* message) {
    Serial.print(message);
    Serial.println(plugged ? "CONNECTED" : "DISCONNECTED");
}

// Notifies when an invalid transmission is detected
void receivedDataError(Wiegand::DataError error, uint8_t* rawData, uint8_t rawBits, const char* message) {
    Serial.print(message);
    Serial.print(Wiegand::DataErrorStr(error));
    Serial.print(" - Raw data: ");
    Serial.print(rawBits);
    Serial.print("bits / ");

    //Print value in HEX
    uint8_t bytes = (rawBits+7)/8;
    for (int i=0; i<bytes; i++) {
        Serial.print(rawData[i] >> 4, 16);
        Serial.print(rawData[i] & 0xF, 16);
    }
    Serial.println();
}
