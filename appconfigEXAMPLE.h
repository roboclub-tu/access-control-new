#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

///////// WIEGAND PINS

#define PIN_D0 33
#define PIN_D1 32

#define PIN_GRN_LED 25
#define PIN_BUZZER 26

///////// BUTTON PINS

#define PIN_ADD_TAG  18
#define PIN_DEL_TAG  19

///////// SENSOR PINS

#define PIN_MAGNET  16
#define PIN_ENDSTOP  17

///////// DATABASE

#define MAX_NUM_OF_TAGS   16

///////// STEP MOTOR

#define PIN_EN  22
#define PIN_DIR  14
#define PIN_STEP  12

//todo implement functionality
#define STEPS_MIN_TO_UNLOCK 750
#define STEPS_FULL_TO_UNLOCK 1050
#define STEPS_BEFORE_LOCKDOWN 1550

///////// INTERNET CONNECTION

#define SSID "your network name"
#define PASSWORD "password"

#define SERVER_NAME "http://192.168.0.0/add_event.php" //replace with your server url
#define SERVER_ADD_TAG "add_tag.php"
#define SERVER_DEL_TAG "del_tag.php"
#define SERVER_ADD_ENTRY "add_new_entry.php"

#define API_KEY "your API key" //sync with php file

#define TIMEOUT_WIFI 10000 //how many ms before abandoning attempt to connect to wifi

#endif // APP_CONFIG_H_
