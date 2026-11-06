#include "commutation.h"

CommutationState_t commutation_map[8] = {
    {GATE_HIZ, GATE_HIZ, GATE_HIZ}, //0b000
    {GATE_PWM, GATE_HIZ, GATE_LOW}, //0b001
    {GATE_HIZ, GATE_PWM, GATE_LOW}, //0b010
    {GATE_LOW, GATE_PWM, GATE_HIZ}, //0b011
    {GATE_LOW, GATE_HIZ, GATE_PWM}, //0b100
    {GATE_HIZ, GATE_LOW, GATE_PWM}, //0b101
    {GATE_PWM, GATE_LOW, GATE_HIZ}, //0b110
    {GATE_HIZ, GATE_HIZ, GATE_HIZ}  //0b111
};

// Mapping is taken from beccas original code
CommutationState_t Commutation_GetGateState(HallState_t hall_state) {
    return commutation_map[hall_state.state];
}

void Commutation_DrivePin(PWMPin_t pin, GateState_t state, float duty) {
    switch (state) {
        case GATE_HIZ:
            HAL_GPIO_WritePin(pin.lo_gpio, pin.lo_gpio_pin, GPIO_PIN_SET);
            break;
        case GATE_LOW:
            pin.tim->Instance->CCR1 = 0; // Set duty cycle to 0
            HAL_GPIO_WritePin(pin.lo_gpio, pin.lo_gpio_pin, GPIO_PIN_RESET);
            break;
        case GATE_HIGH:
            pin.tim->Instance->CCR1 = pin.tim->Instance->ARR + 1; // Set duty cycle to 100
            HAL_GPIO_WritePin(pin.lo_gpio, pin.lo_gpio_pin, GPIO_PIN_RESET);
            break;
        case GATE_PWM:
            pin.tim->Instance->CCR1 = duty * (pin.tim->Instance->ARR + 1);
            HAL_GPIO_WritePin(pin.lo_gpio, pin.lo_gpio_pin, GPIO_PIN_RESET);
            break;
    }
}

void Commutation_UpdateGateDriver(GateDriver_t pins, HallState_t state, float duty) {
    CommutationState_t comm_state = Commutation_GetGateState(state);
    Commutation_DrivePin(pins.a, comm_state.a, duty);
    Commutation_DrivePin(pins.b, comm_state.b, duty);
    Commutation_DrivePin(pins.c, comm_state.c, duty);
}
