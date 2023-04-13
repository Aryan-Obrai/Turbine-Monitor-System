#Turbine Monitor System

Authors: [Aryan Obrai](https://github.com/Ares278)
  
## Project Description

  The Turbine Monitor System showcases how a turbine motor, used for power, can be
monitored via ultrasonic sensors and LCD screens. It serves as a model for larger scale
operations where the concept remains the same, although the parts may vary. The turbine spins
and signals when there is power being generated, and when an object is detected, it stops
spinning until the object is cleared. 

## Functionality

  The system comprises of a joystick that serves as the primary controller input for the user.
To turn the system on or off, the user needs to press the joystick once. To control the speed and
direction of the turbine, the user can use the UP, DOWN, LEFT, and RIGHT buttons on the
joystick. A green LED indicates the presence of power, and a red LED signals that the system is
losing power or there is an object detected. If the green LED is off, the turbine will stop spinning.
When the system is on, the ultrasonic sensor automatically detects objects and outputs the
distance via the LCD screen. The speed and direction of the turbine are also displayed on the
screen. If the system turns off, the LCD will remain blank. If the system loses power, the LCD
will display the last speed, direction, and distance detected.

## Wiring Guide

define RST A5
define CE A4
define DC  2
define DIN 3 
define CLK 4 
define echoPin 8
define trigPin 10
define ENA 9
define INA 6
define INB 7
define RED 12
define GREEN 13
define JS_X = A2;
define JS_Y = A1;
define JS_BTN = 11;
