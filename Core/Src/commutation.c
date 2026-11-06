#include "commutation.h"

// Represents a possible state for an h-bridge
// 3x pwm mode is used
typedef enum {
    GATE_HIZ = 0,
    GATE_LOW,
    GATE_HIGH,
    GATE_PWM
} GateState_t;

GateDriver_t configs[GATE_DRIVER_COUNT] = {0};

// Current duty cycle being ran at, updated via set duty cycle
float duty_cycles[GATE_DRIVER_COUNT] = {0};

const GateState_t *states[GATE_DRIVER_COUNT] = {0};

// Mapping from beccas code of hall to gate driver
// Might depend on motor?
const GateState_t commutation_map[8][3] = {
    {GATE_HIZ, GATE_HIZ, GATE_HIZ}, //0b000
    {GATE_PWM, GATE_HIZ, GATE_LOW}, //0b001
    {GATE_HIZ, GATE_PWM, GATE_LOW}, //0b010
    {GATE_LOW, GATE_PWM, GATE_HIZ}, //0b011
    {GATE_LOW, GATE_HIZ, GATE_PWM}, //0b100
    {GATE_HIZ, GATE_LOW, GATE_PWM}, //0b101
    {GATE_PWM, GATE_LOW, GATE_HIZ}, //0b110
    {GATE_HIZ, GATE_HIZ, GATE_HIZ}  //0b111
};

// Sets a hi+lo pin pair to the given gate state
// If the gate state is pwm, the duty cycle is set to duty
static void Commutation_DrivePin(PWMPin_t pin, GateState_t state, float duty) {
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

static void Commutation_SetGateStates(GateDriverId_t id, const GateState_t states[static 3]) {
    float duty = duty_cycles[id];
    GateDriver_t pins = configs[id];
    Commutation_DrivePin(pins.a, states[0], duty);
    Commutation_DrivePin(pins.b, states[1], duty);
    Commutation_DrivePin(pins.c, states[2], duty);
}

void Commutation_UpdateGateDriver(GateDriverId_t id, HallState_t state) {
    const GateState_t *comm_state = commutation_map[state.state];
    Commutation_SetGateStates(id, comm_state);
    states[id] = comm_state;
}

void Commutation_Init(GateDriverId_t id, GateDriver_t config) {
    configs[id] = config;
    Commutation_SetGateStates(id, commutation_map[0]);
    HAL_TIM_PWM_Start(config.a.tim, config.a.tim_channel);
    HAL_TIM_PWM_Start(config.b.tim, config.b.tim_channel);
    HAL_TIM_PWM_Start(config.c.tim, config.c.tim_channel);
}

void Commutation_SetDuty(GateDriverId_t id, float duty) {
    duty_cycles[id] = duty;
    Commutation_SetGateStates(id, states[id]);
}
