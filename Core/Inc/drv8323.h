#ifndef __DRV8323_H
#define __DRV8323_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32g4xx_hal.h"

// Adapted from Becca's driver

#define DRV8323_COUNT 2

#define DRV8323_ID_DRIVE 0
#define DRV8323_ID_SWERVE 1

typedef struct {
    SPI_HandleTypeDef *spi;

    GPIO_TypeDef *cs_gpio;
    uint16_t cs_pin;

    GPIO_TypeDef *enable_gpio;
    uint16_t enable_pin;

    GPIO_TypeDef *fault_gpio;
    uint16_t fault_pin;
} Drv8323_t;

typedef size_t Drv8323Id_t;

extern Drv8323_t Drv8323_configs[DRV8323_COUNT];

void Drv8323_Init(Drv8323Id_t id, Drv8323_t config);

void Drv8323_SetEnable(Drv8323Id_t id, bool enable);

void Drv8323_SetRegister(Drv8323Id_t id, uint8_t reg, uint16_t value);

uint16_t Drv8323_GetRegister(Drv8323Id_t id, uint8_t reg);

#endif
