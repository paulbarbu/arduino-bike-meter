Bike-Meter
==========
Note: work in progress

A simple speed and distance bike computer built on Arduino Nano, showing the data on a 16x2 LCD display, featuring automatic display brightness adjustment.

If you're using your bike on a home trainer and don't have the necessary speed sensors on it, this might help.
It uses a hall sensor connected to the Arduino Nanoa to count your back wheel's rotations and from there it computes the speed and distance
traveled for the current training session.

So you need to place the hall sensor near the roller of your home trainer and using long enough wires, place the arduino and
the LCD display near the cockpit of the bike.
You'll also need to attach a magnet to your home trainer's roller and near the afore mentioned hall sensor.

*NOTE:* Make sure nothing will get stuck to your wheel, cables and such. Do not damage your equipment or yourself.

Usage
=====
You may need to change the `WHEEL_DIAMETER` to reflect you wheel's diameter in millimeters for accurate computations.
Do the same for `TRAINER_ROLLER_DIAMETER`.

If no rotations are detected, you need to decrease the distance between the magnet and the hall sensor.

Flash on the arduino, attach to trainer and power the arduino.
There is no start/stop button, nor does it store the data computed during the training session.
From the power-up moment, the arduino starts doing its job.

In order to avoid keeping your laptop near the bike after flashing the arduino, you may power the arduino from a power bank or using the pins dedicated to an external power supply.

Parts
=====
* Arduino Nano
* TLE4906L Hall effect sensor
* 16x2 LCD
* LCM 1602 I2C module for the 16x2 LCD
* 1x 220 Ohm resistor
* 2x 10 kOhm resistors
* Phototresistor
* LM35 temperature sensor

Wiring details can be found in the code itself and in the image below:
![sketch_bb.png - Parts wiring details](/sketch_bb.png?raw=true "Wiring details")

LICENSE
=======
See the LICENSE file (MIT).
