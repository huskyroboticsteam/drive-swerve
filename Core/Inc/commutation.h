#ifndef __COMMUTATION_H
#define __COMMUTATION_H

#include <stdint.h>
#include "hall_encoders.h"
#include "stm32g4xx_hal.h"

#define GATE_DRIVER_COUNT 2

#define DRIVER_ID_DRIVE 0
#define DRIVER_ID_SWERVE 1

typedef struct {
    TIM_HandleTypeDef *tim;
    uint32_t tim_channel;
    GPIO_TypeDef *lo_gpio;
    uint16_t lo_gpio_pin;
} PWMPin_t;

typedef struct {
    PWMPin_t a;
    PWMPin_t b;
    PWMPin_t c;
} GateDriver_t;

typedef size_t GateDriverId_t;

// Updates the gate driver (using 3x pwm mode) based on the current hall state
void Commutation_UpdateGateDriver(GateDriverId_t id, HallState_t state);

void Commutation_Init(GateDriverId_t id, GateDriver_t config);

void Commutation_SetDuty(GateDriverId_t id, float duty);

// Represents a possible state for an h-bridge
// 3x pwm mode is used
typedef enum {
    GATE_HIZ = 0,
    GATE_LOW,
    GATE_HIGH,
    GATE_PWM
} GateState_t;

// Sets a hi+lo pin pair to the given gate state
// If the gate state is pwm, the duty cycle is set to duty
void Commutation_DrivePin(PWMPin_t pin, GateState_t state, float duty);

#endif //__COMMUTATION_H
