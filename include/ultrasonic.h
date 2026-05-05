#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>

void ultrasonic_init(void);
uint16_t ultrasonic_read_cm(void);

#endif
