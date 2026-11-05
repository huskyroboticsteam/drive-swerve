#include "commutation.h"

// Mapping is taken from beccas original code
CommutationState_t Commutation_GetGateState(HallState_t hall_state) {
    switch (hall_state.state) {
        case 0:
            // Invalid
            return (CommutationState_t){0};
        case 1:
            return (CommutationState_t){
                .a = GATE_PWM,
                .c = GATE_LOW
            };
         case 2:
            return (CommutationState_t){
                .b = GATE_PWM,
                .c = GATE_LOW
            };
         case 3:
            return (CommutationState_t){
                .a = GATE_LOW,
                .b = GATE_PWM
            };
         case 4:
            return (CommutationState_t){
                .a = GATE_LOW,
                .c = GATE_PWM
            };
         case 5:
            return (CommutationState_t){
                .b = GATE_LOW,
                .c = GATE_PWM
            };
         case 6:
            return (CommutationState_t){
                .a = GATE_PWM,
                .b = GATE_LOW
            };
         case 7:
            // Invalid
            return (CommutationState_t){0};
         default:
            __builtin_unreachable();
    }
}
