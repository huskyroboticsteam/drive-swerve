#ifndef __CURRENT_SENSE_H
#define __CURRENT_SENSE_H

#include <stdint.h>
#include "stm32g4xx_hal.h"

#define CURRENT_SENSE_MAX 2u // translation + rotation
#define GATE_DRIVER_COUNT 3u

#define DRIVER_ID_DRIVE 0u // might not need
#define DRIVER_ID_SWERVE 1u // might not need

#define PHASE_COUNT 3u // might not need

#define R_SHUNT      0.001f // 1 ohm shunt resistor, might need to  revisit
#define CSA_GAIN     20.0f // might need to revisit along with shunt resistor
#define ADC_VREF     3.3f
#define ADC_MAX_CNT  4096.0f
#define A_PER_LSB  (ADC_VREF / ADC_MAX_CNT / (CSA_GAIN * R_SHUNT))


typedef struct {
    ADC_HandleTypeDef *hadc; // NULL = slot unused
    uint16_t offset[PHASE_COUNT];
    volatile float phase_currents[PHASE_COUNT];
} ADC_CurrentSense_t;

static HAL_StatusTypeDef read_raw(ADC_HandleTypeDef *hadc, uint16_t raw[3]);
HAL_StatusTypeDef CurrentSense_Init(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef CurrentSense_Get(ADC_HandleTypeDef *hadc);

#endif // CURRENT_SENSE_H
