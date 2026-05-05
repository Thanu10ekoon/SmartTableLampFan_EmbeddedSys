#ifndef PRESENCE_H
#define PRESENCE_H

#include <stdint.h>

typedef struct {
    uint8_t active;
    uint16_t on_cm;
    uint16_t off_cm;
} PresenceState;

void presence_init(PresenceState *state, uint16_t on_cm, uint16_t off_cm);
void presence_update(PresenceState *state, uint8_t motion, uint16_t distance_cm);

#endif
