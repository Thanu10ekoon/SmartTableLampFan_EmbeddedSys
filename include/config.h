#ifndef CONFIG_H
#define CONFIG_H

#define F_CPU 16000000UL

#include <avr/io.h>

// Pin assignments
#define LED_PIN PB5
#define PIR_PIN PD2
#define MOTOR_PIN PD3
#define TRIG_PIN PD4
#define ECHO_PIN PD5

// ADC channel for LDR
#define LDR_ADC_CH 0

// Thresholds
#define LIGHT_THRESHOLD 500
#define PRESENCE_ON_CM 10
#define PRESENCE_OFF_CM 58

#endif
