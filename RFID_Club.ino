
#include <Wiegand.h>
#include <EEPROM.h>
#include <RfidDb.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "step_motor.h"
#include "appconfig.h"


// RFID reader instance
Wiegand wiegand;
// Database instance
RfidDb database = RfidDb(MAX_NUM_OF_TAGS, 0);
// Stepper motor instance
StepMotor stepper;

//FIXME bandage fix, see what var is needed for http.begin()
String serverName = SERVER_NAME;
String apiKey = API_KEY;

void setup() {
  Serial.begin(115200);

  EEPROM.begin(database.dbSize());
  database.begin();
  setupWiegand(); // Initialize Wiegand reader

  long abandonTime = millis() + TIMEOUT_WIFI;

  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED && millis() < abandonTime) {
    delay(500);
    Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Couldn't connect to WiFi, continuing in offline mode");
  }

  printSensorData();
  Serial.println("SETUP FINISHED");
}

// Every few milliseconds, check for pending messages on the wiegand reader
// This executes with interruptions disabled, since the Wiegand library is not thread-safe
void loop() {
  noInterrupts();
  wiegand.flush(); //library is not thread safe (interrupt based) and this method can fail if it's interrupted. Therefore, we need to turn off interrupts
  //flush is needed, because we don't have a set tag size
  
  interrupts();
  //Sleep a little -- this doesn't have to run very often.
  delay(100); //needed if we use automatic tag size detection
}

/////////////// WIEGAND LOGIC /////////////////

//TODO this should probably be in a different object in the future

// Notifies when a card was read.
// Instead of a message, the seconds parameter can be anything you want -- Whatever you specify on `wiegand.onReceive()`
void receivedData(uint8_t* rawData, uint8_t bits, const char* message) {
    String tag_hex = getHex(rawData, bits);
    printTagMessage(tag_hex, bits, message);

    //Converting to uint32_t so it's database friendly
    uint32_t dbTag = stream2int(rawData);
    Serial.print("uint32_t tag = ");
    Serial.println(dbTag);

    //Check if add tag button is pressed, then add tag to DB
    if (digitalRead(PIN_ADD_TAG) == HIGH) {
      if (database.insert(dbTag)) {
        Serial.println("Inserted or already existed in local DB");
        
        if(sendToServer(tag_hex, SERVER_ADD_TAG)){
          Serial.println("Succesfully added in Server");
        } else {
          Serial.println("Error writing to server!");
        }
        
      } else {
        Serial.println("Insert in local DB failed");
      }
    }
    
    //Check if deltag button is pressed, then delete tag from DB
    else if (digitalRead(PIN_DEL_TAG) == HIGH) {
      database.remove(dbTag);
      Serial.println("Deleted or didn't exist in local DB");

      if(sendToServer(tag_hex, SERVER_DEL_TAG)){
          Serial.println("Succesfully deleted from Server");
      } else {
          Serial.println("Error writing to server!");
      }
    }
    //else check if tag is in DB, if yes: open door, else : access denied 
    else {
      if (database.contains(dbTag)) {
        Serial.println("In DB");
        Serial.println("Changing lock state...");
        stepper.changeLockState();
      } else {
        Serial.println("NOT in DB");
      }
      
      //TODO test if multithreading is needed (if proccess is too slow)
      if(sendToServer(tag_hex, SERVER_ADD_ENTRY)){
          Serial.println("Succesfully added entry in Server");
      } else {
          Serial.println("Error writing to server!");
      }
    }
}

void setupWiegand() {
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
    String hex = getHex(rawData, rawBits);
    Serial.print(Wiegand::DataErrorStr(error));
    printTagMessage(hex, rawBits, message);
    
}

//Conversion from format Wiegand is using to format DB is using
static inline uint32_t stream2int(const uint8_t *stream) {
    return (((uint32_t) stream[0]) << 24 |
            ((uint32_t) stream[1]) << 16 |
            ((uint32_t) stream[2]) <<  8 |
            ((uint32_t) stream[3]) <<  0);
}

void printTagMessage(String hex, uint8_t bits, const char* message) {
    Serial.print(message);
    Serial.print(bits);
    Serial.print("bits / ");
    Serial.println(hex);
}

//TODO test if new hex method works
String getHex(uint8_t* rawData, uint8_t bits) {
    String hex = "";
    
    uint8_t bytes = (bits+7)/8;
    for (int i=0; i<bytes; i++) {
      hex += String(rawData[i] >> 4, 16);
      hex += String(rawData[i] & 0xF, 16);
    }

    //TODO remove
    Serial.println("Generated hex: " + hex);

    //TODO make toupper
    
    return hex;
}

void printSensorData() {
  Serial.print("Magnetic sensor: ");
  if(digitalRead(PIN_MAGNET)) {
    Serial.println("CLOSED");
  } else {
    Serial.println("OPENED");
  }

  Serial.print("Endstop sensor: ");
  if(digitalRead(PIN_ENDSTOP)) {
    Serial.println("NOT PRESSED");
  } else {
    Serial.println("PRESSED");
  }
}

//sends an http request containing a tag hex to the absolute path (foo.php)
bool sendToServer(String tag, String path) {
  //proceed only if connected
    if(WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      http.begin(serverName + path);
      Serial.println("Server path:" + serverName + path);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String httpRequestData = "ApiKey=" + apiKey + "&Tag=" + tag;

      Serial.print("HTTP Request: ");
      Serial.println(httpRequestData);
      
      int httpResponseCode = http.POST(httpRequestData);

      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);

      http.end();

      if(httpResponseCode == 200) {
        return true;
      } else {
        return false;
      }
      
    } else {
      Serial.println("WiFi not connected");
      return false;
    }
}
