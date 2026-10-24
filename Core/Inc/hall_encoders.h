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
} HallState_t;

typedef struct {
    TIM_HandleTypeDef *timer;
    GPIO_TypeDef *a_gpio;
    GPIO_TypeDef *b_gpio;
    GPIO_TypeDef *c_gpio;
} HallConfig_t;

typedef size_t HallId_t;

extern volatile HallState_t HallStates[HALL_COUNT];

void Hall_Init(HallId_t id, HallConfig_t config);

#endif // __HALL_ENCODERS_H
