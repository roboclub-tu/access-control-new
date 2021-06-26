# access-control-new
Access control system with RFID card reader for the Robotics club in TU Sofia.

The door system consists of:
1. RFID card reader (Wiegand based)
2. Access control lock (Step motor)
3. EEPROM(Flash memory for ESP) based tag database
4. Button based tag adding/deleting

The networking system consists of:
1. Raspberry Pi with installed LAMP (Linux Apache Mysql PHP)

# Hardware used

- [ESP32 DEVKIT C-V4](https://www.espressif.com/en/products/devkits/esp32-devkitc/overview)
- [Wiegand 34](https://www.banggood.com/Long-Range-RFID-Card-Reader-13_56MHZ125KHZ-Proximity-Card-Access-Control-Reader-Wiegand34-IP65-Waterproof-NFC-Reader-p-1534554.html) chinese RFID card reader on 13.56 Mhz
- [Nema 17](https://erelement.com/motori-reduktori/stepper-motor-12) bipolar stepper motor
- [Pololu A4988](https://erelement.com/motor-control/motor-driver-a4988) driver for stepper motors
- [Mech endstop V1.2](https://www.amazon.com/INCREWAY-Mechanical-Endstop-Printer-Makerbot/dp/B07JGFRSNJ) to determine when the door is locked
- [Generic magnetic door sensor](http://www.entrypass.net/w3v1/products/accessories/magnetic-door-sensor) to determine if the door is closed
- [Raspberry Pi 3](https://www.raspberrypi.org/products/raspberry-pi-3-model-b) server listening for events from ESP and displaying information to the user

# Technical specification 
[RFID system plan and specifications](https://docs.google.com/document/d/1yX86LziYzrwqBx-fCvdq2RcesHaJ3FzYKwYy3UPbjlI)

# Environment

- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Visual Studio Code](https://code.visualstudio.com/download) with Platform.io extension
- [EasyEda](https://easyeda.com/page/download) for schematics
- [SolidWorks](https://www.solidworks.com/) Modeling 3d parts

# Schematics

![alt text](https://cdn.discordapp.com/attachments/332861936926588929/834723029325709312/Schematic_test-access-control_2021-04-22_12-30-51.png)

# 3D printed parts

[Case for RaspBerry pi](https://www.thingiverse.com/thing:922740)

# How the system works

RFID tag - chip that you get close to the RFID reader, contains an unique identifier, which is then converted to HEX for easier visualisation.
t_int8,16,32 - essentially a byte, short and int. Number shows how many bits of memory are dedicated for the integer.

1. Opening and closing the door

When you get the RFID tag close to the reader, the ESP reads the tag via the Wiegand library (interrupt based, therefore not thread-safe). The tag is saved as an t_int8. The database uses t_int32 to save the tags, so we convert it to t_int32.

If add tag button is pressed (digitalRead), we use the insert method of the database library. If it has been inserted or it already exists, it returns true. If insertion in local DB has been succesful, the SendToServer(SERVER_ADD_TAG) method is called (see Networking).

If delete tag button is pressed (digitalRead), we use the remove method of the database library. In its current version, it does NOT return whether deleting is succesful. After remove is executed, sendToServer(SERVER_DEL_TAG) method is called (See Networking).

If no button is pressed, the program proceeds to using the StepMotor object and the changeLockState method. After executing changeLockState(), sendToServer(SERVER_ADD_ENTRY) method is called (See Networking).


2. changeLockState() method

the StepMotor library is based on 3 hardware components - Endstop sensor (checks when the door is locked), magnetic door sensor (checks if the door is opened) and the step motor itself (drives the lock).

(ENDSTOP REACHED, DOOR CLOSED) - The door is locked. When using the changeLockState() method, the lock will move with STEPS_FULL_TO_UNLOCK number of steps to unlock the door
(ENDSTOP REACHED, DOOR OPENED) - Not programmed, you can only reach this state if one of the sensors is broken
(ENDSTOP NOT REACHED, DOOR OPENED) - The door is currently opened, so the tag can be read, added/deleted from DB and the ESP can connect to the server, but after the StepMotor library sees the door sensor is indicating open door, it prints that the state is not in state to be moved
(ENDSTOP NOT REACHED, DOOR CLOSED) - The stepsToStopper() method is called, that counts how many steps are made until the endstop sensor is reached and returns it. 
If the door has been opened while stepsToStopper() is being executed, the lock moves to the opposite direction with the number of steps made so far. The method returns -1 steps made after that.
If there are more steps made than the STEPS_BEFORE_LOCKDOWN in appconfig.h, the check to steps method returns STEPS_BEFORE_LOCKDOWN and stops moving the lock.

After we have the number of steps, if they are bigger than STEPS_MIN_TO_UNLOCK, it means the lock WAS in locked state. if it was lower than STEPS_MIN_TO_UNLOCK, it means it WAS in unlocked state.

After we know the before state of the lock, we can unlock it(steps>STEPS_MIN_TO_UNLOCK) or leave it (steps<STEPS_MIN_TO_UNLOCK), since in the stepsToStopper() method, we already locked it.

Unlocking the door means moving it with STEPS_FULL_TO_UNLOCK steps in the opposite direction.



3. Networking

In the appconfig file, there is information about the server path (local ip in this example) and relative paths to different php files (add_tag.php, del_tag.php, add_new_entry.php). They all take 2 arguments via POST, which are the API key and tag hex. Each of these represents a different event from the ESP. There is also information about the WiFi network (SSID, password) the ESP will connect to.

during the setup of the ESP, it will try to connect to the WiFi network. If it can't for more then the TIMEOUT_WIFI ms (from appconfig.h), it will continue on in offline mode, while trying to connect in the background.

!Sometimes the ESP bugs and refuses to connect to the WiFi network, a restart of the ESP solves that problem. 

When the ESP registers an event (adding/deleting a tag, changing lock state), the SendToServer() method is called from within the event. This method takes 1 argument, which is the relative path to the needed .php file. If add_tag button is pressed for example, the method is called with sendToServer(SERVER_ADD_TAG).


