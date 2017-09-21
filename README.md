# Joystick-Controlled-Wheelchair-BLDC
The repo consists of Joystick controller Wheelchair Firmware using Arduino Mega and BLDC HUB motors.
Courtesy: http://www.speedlimit88.com/arduino/adc2dac/
**Hardware**
------ 

* Arduino Mega
*  DAC controller SPI interface-  MCP4921
* 250W BLDC HUB motor driver

-----


**Connections**
----
* DAC Pin Connections
*  Chip select1 - 10
*  Chip select2 -  8
*  SPI MOSI -51
*  SPI CLK -52

*  breakLeft - 14                 
*  breakRight - 15;
*  reverseLeft_A - 44
*  reverseRight_A - 45; 
*  throttle1 -o/p of DAC IC1
*  throttle2 -o/p of DAC IC2

Joystick Connections:
* joysticYA = A0; //Analog Jostick Y axis
* byte joysticXA = A1; //Analog Jostick X axis

Joystick used in this project is Simple analog Joystick made of potential divider or hall sensors

