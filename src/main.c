#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// ---------- ADC SETUP ----------
void ADC_init() {
    ADMUX = (1 << REFS0);  // AVcc reference
    ADCSRA = (1 << ADEN)
           | (1 << ADPS2) | (1 << ADPS1); // prescaler 64
}

// ---------- READ ADC ----------
uint16_t ADC_read(uint8_t ch) {
    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

// ---------- ULTRASONIC PINS ----------
#define TRIG_PIN PD4
#define ECHO_PIN PD5

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

int main(void)
{
    // LED (D13)
    DDRB |= (1 << PB5);

    // PIR input (D2)
    DDRD &= ~(1 << PD2);

    // Motor output (D3)
    DDRD |= (1 << PD3);

    ADC_init();
    ultrasonic_init();

    uint16_t lightThreshold = 500;
    uint8_t ledState = 0;

    // This is the important latch:
    // 0 = inactive, 1 = PIR has started presence and ultrasonic maintains it
    uint8_t presenceActive = 0;

    // Hysteresis thresholds for ultrasonic
    uint16_t presenceOnCm = 15;
    uint16_t presenceOffCm = 55;

    while (1)
    {
        uint8_t motion = (PIND & (1 << PD2)) ? 1 : 0;
        uint16_t lightValue = ADC_read(0);
        uint8_t isDark = (lightValue < lightThreshold);

        // PIR is the ONLY thing that can start presence
        if (motion) {
            presenceActive = 1;
        }

        // Ultrasonic is only allowed to decide AFTER PIR has started it
        if (presenceActive) {
            uint16_t distance = ultrasonic_read_cm();

            if (distance == 0xFFFF || distance >= presenceOffCm) {
                presenceActive = 0;   // person moved away
            }
            else if (distance <= presenceOnCm) {
                presenceActive = 1;   // person still near
            }
        }

        // -------- MOTOR CONTROL (presence only) --------
        if (presenceActive) {
            PORTD |= (1 << PD3);   // Motor ON
        } else {
            PORTD &= ~(1 << PD3);  // Motor OFF
        }

        // -------- LED CONTROL (presence + LDR) --------
        if (presenceActive && isDark) {
            PORTB |= (1 << PB5);   // LED ON
            ledState = 1;
        } else {
            PORTB &= ~(1 << PB5);  // LED OFF
            ledState = 0;
        }

        _delay_ms(100);
    }
}