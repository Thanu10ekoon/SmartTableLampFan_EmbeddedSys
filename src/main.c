#include "config.h"
#include "adc.h"
#include "presence.h"
#include "ultrasonic.h"

#include <util/delay.h>

int main(void)
{
    // LED (D13)
    DDRB |= (1 << LED_PIN);

    // PIR input (D2)
    DDRD &= ~(1 << PIR_PIN);

    // Motor output (D3)
    DDRD |= (1 << MOTOR_PIN);

    ADC_init();
    ultrasonic_init();

    uint16_t lightThreshold = LIGHT_THRESHOLD;

    // 0 = inactive, 1 = PIR has started presence and ultrasonic maintains it
    PresenceState presence;
    presence_init(&presence, PRESENCE_ON_CM, PRESENCE_OFF_CM);

    while (1)
    {
        uint8_t motion = (PIND & (1 << PIR_PIN)) ? 1 : 0;
        uint16_t lightValue = ADC_read(LDR_ADC_CH);
        uint8_t isDark = (lightValue < lightThreshold);

        uint16_t distance = 0xFFFF;
        if (presence.active || motion) {
            distance = ultrasonic_read_cm();
        }
        presence_update(&presence, motion, distance);

        // -------- MOTOR CONTROL (presence only) --------
        if (presence.active) {
            PORTD |= (1 << MOTOR_PIN);   // Motor ON
        } else {
            PORTD &= ~(1 << MOTOR_PIN);  // Motor OFF
        }

        // -------- LED CONTROL (presence + LDR) --------
        if (presence.active && isDark) {
            PORTB |= (1 << LED_PIN);   // LED ON
        } else {
            PORTB &= ~(1 << LED_PIN);  // LED OFF
        }

        _delay_ms(100);
    }
}