#ifndef __HALL_ENCODERS_H
#define __HALL_ENCODERS_H

#include <stdint.h>
#include "stm32g4xx_hal.h"

// Number of hall encoders on the board
#define HALL_COUNT 2

// Ids for hall encoders
#define HALL_ID_DRIVE 0
#define HALL_ID_SWERVE 1

typedef struct {
    uint8_t a : 1;
    uint8_t b : 1;
    uint8_t c : 1;
    uint8_t _padding : 5;
} HallState_t;

typedef struct {
    TIM_HandleTypeDef *timer;
    GPIO_TypeDef *a_gpio;
    uint16_t a_gpio_pin;
    GPIO_TypeDef *b_gpio;
    uint16_t b_gpio_pin;
    GPIO_TypeDef *c_gpio;
    uint16_t c_gpio_pin;
} HallConfig_t;

typedef size_t HallId_t;

extern volatile HallState_t Hall_States[HALL_COUNT];

// Initializes a hall encoder
// Should not have null entries in the config
// id should be in the range [0,  HALL_COUNT)
void Hall_Init(HallId_t id, HallConfig_t config);

// Updates a hall encoder by reading GPIO
// id should be in the range [0, HALL_COUNT)
void Hall_Update(HallId_t id);

#endif // __HALL_ENCODERS_H
