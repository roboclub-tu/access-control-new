/*
 * kill me
 */

#include <Wiegand.h>
#include <EEPROM.h>
#include <RfidDb.h>
//#include <step_motor.h>
#include <appconfig.h>


// RFID reader instance
Wiegand wiegand;
// Database instance
RfidDb database = RfidDb(MAX_NUM_OF_TAGS, 0);

// Initialize Wiegand reader
void setup() {
  Serial.begin(115200);

  EEPROM.begin(database.dbSize());
  database.begin();

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

  Serial.println("SETUP FINISHED");
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

// Notifies when a card was read.
// Instead of a message, the seconds parameter can be anything you want -- Whatever you specify on `wiegand.onReceive()`
void receivedData(uint8_t* rawData, uint8_t bits, const char* message) {
    Serial.print(message);
    Serial.print(bits);
    Serial.print("bits / ");
    //Print value in HEX
    uint8_t bytes = (bits+7)/8;
    for (int i=0; i<bytes; i++) {
        Serial.print(rawData[i] >> 4, 16);
        Serial.print(rawData[i] & 0xF, 16);
    }
    Serial.println();

    //Converting to uint32_t so it's database friendly
    uint32_t dbTag = stream2int(rawData);
    Serial.print("uint32_t tag = ");
    Serial.println(dbTag);

    //Check if add tag button is pressed, then add tag to DB
    if (digitalRead(PIN_ADD_TAG) == HIGH) {
      if (database.insert(dbTag)) {
        Serial.println("Inserted or already existed");
      } else {
        Serial.println("Insert failed");
      }
    }
    //Check if deltag button is pressed, then delete tag from DB
    else if (digitalRead(PIN_DEL_TAG) == HIGH) {
      database.remove(dbTag);
      Serial.println("Deleted or didn't exist");
    }
    //else check if tag is in DB, if yes: open door, else : access denied 
    else {
      if (database.contains(dbTag)) {
        Serial.println("In DB");
      } else {
        Serial.println("NOT in DB");
      }
    }
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

//Conversion from format Wiegand is using to format DB is using
static inline uint32_t stream2int(const uint8_t *stream) {
    return (((uint32_t) stream[0]) << 24 |
            ((uint32_t) stream[1]) << 16 |
            ((uint32_t) stream[2]) <<  8 |
            ((uint32_t) stream[3]) <<  0);
}