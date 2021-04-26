# access-control-new
Access control system with RFID card reader for the Robotics club in TU Sofia.

The system consists of:
1. RFID card reader (Wiegand or RFID 522)
2. Access control lock (Electromagnetic lock or Step motor)
3. EEPROM based tag database
4. Button or WiFi based tag adding/deleting

# Hardware used

- [ESP32 DEVKIT C-V4](https://www.espressif.com/en/products/devkits/esp32-devkitc/overview)
- [Wiegand 34](https://www.banggood.com/Long-Range-RFID-Card-Reader-13_56MHZ125KHZ-Proximity-Card-Access-Control-Reader-Wiegand34-IP65-Waterproof-NFC-Reader-p-1534554.html) chinese RFID card reader on 13.56 Mhz
- [Nema 17](https://erelement.com/motori-reduktori/stepper-motor-12) bipolar stepper motor
- [Pololu A4988](https://erelement.com/motor-control/motor-driver-a4988) driver for stepper motors
- [Mech endstop V1.2](https://www.amazon.com/INCREWAY-Mechanical-Endstop-Printer-Makerbot/dp/B07JGFRSNJ) to determine when the door is locked
- [Generic magnetic door sensor] to determine if the door is closed

# Technical specification 
[RFID system plan and specifications](https://docs.google.com/document/d/1yX86LziYzrwqBx-fCvdq2RcesHaJ3FzYKwYy3UPbjlI)

# Environment

- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Visual Studio Code](https://code.visualstudio.com/download) with Arduino extension
- [EasyEda](https://easyeda.com/page/download) for schematics

# Schematics

![alt text](https://cdn.discordapp.com/attachments/332861936926588929/834723029325709312/Schematic_test-access-control_2021-04-22_12-30-51.png)

# 3D printed case for the stepper motor

Coming soon ..
