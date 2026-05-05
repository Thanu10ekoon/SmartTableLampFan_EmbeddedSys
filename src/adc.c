#include "adc.h"
#include <avr/io.h>

void ADC_init(void) {
    ADMUX = (1 << REFS0);  // AVcc reference
    ADCSRA = (1 << ADEN)
           | (1 << ADPS2) | (1 << ADPS1); // prescaler 64
}

uint16_t ADC_read(uint8_t ch) {
    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {
    }
    return ADC;
}
