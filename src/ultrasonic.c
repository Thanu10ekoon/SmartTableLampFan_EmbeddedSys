#include "config.h"
#include "ultrasonic.h"
#include <util/delay.h>

void ultrasonic_init(void) {
    DDRD |= (1 << TRIG_PIN);     // TRIG output
    DDRD &= ~(1 << ECHO_PIN);    // ECHO input
    PORTD &= ~(1 << TRIG_PIN);
}

uint16_t ultrasonic_read_cm(void) {
    uint16_t timeout = 0;
    uint16_t pulse_us = 0;

    // Trigger pulse
    PORTD &= ~(1 << TRIG_PIN);
    _delay_us(2);
    PORTD |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTD &= ~(1 << TRIG_PIN);

    // Wait for ECHO HIGH
    timeout = 0;
    while (!(PIND & (1 << ECHO_PIN))) {
        _delay_us(1);
        timeout++;
        if (timeout > 30000) {
            return 0xFFFF;
        }
    }

    // Measure HIGH width
    pulse_us = 0;
    while (PIND & (1 << ECHO_PIN)) {
        _delay_us(1);
        pulse_us++;
        if (pulse_us > 30000) {
            return 0xFFFF;
        }
    }

    return pulse_us / 58;   // cm
}
