#ifndef __COMMUTATION_H
#define __COMMUTATION_H

#include "hall_encoders.h"

typedef enum {
    GATE_HIZ,
    GATE_LOW,
    GATE_HIGH,
    GATE_PWM
} GateState_t;

typedef struct {
    GateState_t a;
    GateState_t b;
    GateState_t c;
} CommutationState_t;

CommutationState_t Commutation_GetGateState(HallState_t hall_state);

#endif //__COMMUTATION_H
