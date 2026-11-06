#ifndef __COMMUTATION_H
#define __COMMUTATION_H

#include <stdint.h>
#include "hall_encoders.h"
#include "stm32g4xx_hal.h"

// Represents a possible state for an h-bridge
// 3x pwm mode is used
typedef enum {
    GATE_HIZ = 0,
    GATE_LOW,
    GATE_HIGH,
    GATE_PWM
} GateState_t;

// Tracks the full 3x pwm state
typedef struct {
    GateState_t a;
    GateState_t b;
    GateState_t c;
} CommutationState_t;

// Converts a hall state into the corresponding commutation pattern
// Returns 3 states, one for each of the h-bridges
// Might depend on the motor?
CommutationState_t Commutation_GetGateState(HallState_t hall_state);

typedef struct {
    TIM_HandleTypeDef *tim;
    uint32_t tim_channel;
    GPIO_TypeDef *lo_gpio;
    uint16_t lo_gpio_pin;
} PWMPin_t;

// Sets a hi+lo pin pair to the given gate state
// If the gate state is pwm, the duty cycle is set to duty
void Commutation_DrivePin(PWMPin_t pin, GateState_t state, float duty);

typedef struct {
    PWMPin_t a;
    PWMPin_t b;
    PWMPin_t c;
} GateDriver_t;

// Combines the functionality of the above functions
// Used to update all 6 pins based on the hall encoder
void Commutation_UpdateGateDriver(GateDriver_t pins, HallState_t state, float duty);

#endif //__COMMUTATION_H
