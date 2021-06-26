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

If add tag button is pressed (digitalRead), we use the insert method of the database library. If it has been inserted or it already exists, it returns true.

If delete tag button is pressed (digitalRead), we use the remove method of the database library. In its current version, it does NOT return whether deleting is succesful.

If no button is pressed, the program proceeds to using the StepMotor object and the changeLockState method.

2. changeLockState() method


3. Networking

The system consists of a *door* client and a raspberry server listener. When the *door* registers an event (scanning the RFID tag), it sends information to the server. This event may be:
  1. Adding a new tag
  2. Deleting a tag (may or may not be written in the ESP database)
  3. Changing the lock state (when you just scan the RFID tag)

In the appconfig file, there is information about the server path (local ip in this example) and relative paths to different php files (add_tag.php, del_tag.php, add_new_entry.php). They all take 2 arguments via POST, which are the API key and tag hex. Each of these represents a different event from the ESP and are connected to according to the ESP event.


