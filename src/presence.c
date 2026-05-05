#include "presence.h"

void presence_init(PresenceState *state, uint16_t on_cm, uint16_t off_cm) {
    state->active = 0;
    state->on_cm = on_cm;
    state->off_cm = off_cm;
}

void presence_update(PresenceState *state, uint8_t motion, uint16_t distance_cm) {
    // PIR is the only thing that can start presence
    if (motion) {
        state->active = 1;
    }

    // Ultrasonic is only allowed to decide after PIR has started it
    if (state->active) {
        if (distance_cm == 0xFFFF || distance_cm >= state->off_cm) {
            state->active = 0;
        } else if (distance_cm <= state->on_cm) {
            state->active = 1;
        }
    }
}
