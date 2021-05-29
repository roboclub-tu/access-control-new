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

///////// INTERNET CONNECTION

#define SSID "VIVACOM_NET"
#define PASSWORD "12345678"

#define SERVER_NAME "http://168.192.1.105:80/ESP/esp_data_handler.php"

#define API_KEY "r2543fgo2" //sync with php file

#endif // APP_CONFIG_H_
