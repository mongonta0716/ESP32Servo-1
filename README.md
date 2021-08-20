# ESP32Servo [![Build Status](https://travis-ci.org/ShellAddicted/ESP32Servo.svg?branch=master)](https://travis-ci.org/ShellAddicted/ESP32Servo)
This idf-component permits to control hobby-grade servo motors using an [Espressif's ESP32 SoC](https://www.espressif.com/en/products/hardware/esp32/overview) (running [esp-idf](https://github.com/espressif/esp-idf))

Tested with [TowerPro - SG92R](http://www.towerpro.com.tw/product/sg92r-7/) and ESP32D0WDQ6 (DevKitC).

# Getting Started
<b>NOTE: this code is not (yet) Production Ready.</b>  
You can use this library as a component for your project:  
```
cd <YOUR_PROJECT_ROOT>
mkdir components/
cd components/
git clone https://github.com/ShellAddicted/ESP32Servo.git
```
for more details see [examples/](https://github.com/ShellAddicted/ESP32SimpleServo/tree/master/examples)  

# Usage of smoothMove
Decide the start and end points, the travel time, and the angle to move (default is 1°) to make the servo move smoothly.

Note that if the time for the servo to move finely is less than 10msec, the operation time will be shifted. (This is a specification of vTaskDelay.)

Specify and use the following four parameters.
- start degree(0〜180°)
- stop degree(0〜180°)
- milliseconds for move
- minimum degree(default: 1°)

## smoothMoveDirect
The starting point moves from one previous position.
