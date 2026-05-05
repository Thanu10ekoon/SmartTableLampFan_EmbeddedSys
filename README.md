# Smart Table Lamp + Fan (att2)

## Overview
This project is a compact embedded system for a desk workspace. When a person sits in front of the table, the system automatically turns on a fan. If the ambient light is not sufficient, it also turns on a lamp. The lamp stays off when there is enough light in the environment.

## Behavior
- Detects presence at the desk using a PIR sensor and an ultrasonic sensor.
- Turns the fan ON when presence is detected.
- Checks ambient light using an LDR.
- Turns the lamp ON only if light level is below the threshold.
- Keeps the lamp OFF when ambient light is sufficient.

## Inputs
- PIR sensor (motion/presence)
- Ultrasonic sensor (distance/presence)
- LDR (ambient light level)

## Outputs
- Fan (through a relay)
- LED array lamp (through a relay)

## Hardware Summary
- Microcontroller: Arduino Uno (PlatformIO environment)
- Sensors: PIR, Ultrasonic, LDR
- Actuators: Fan, LED array
- Switching: Relay module(s)

## Notes
- Thresholds for light and presence can be tuned in firmware.
- Ensure proper power isolation when switching the fan and lamp via relays.
