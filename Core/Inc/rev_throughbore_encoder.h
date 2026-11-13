#ifndef __REV_THROUGHBORE_ENCODER_H
#define __REV_THROUGHBORE_ENCODER_H

#include "stm32g4xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim; // Pointer to the timer handle
    int32_t position;        // Total accumulated position with overflow
    uint32_t last_counter;   // Previous hardware counter value
    uint32_t last_time_read;
    uint16_t delta_time;
    float velocity;
} Quadrature_Encoder_t;

typedef struct {
    TIM_HandleTypeDef *htim; // Pointer to the timer handle
} Absolute_Encoder_t;

// Absolute Encoder Functions
void Absolute_EncoderInit(Absolute_Encoder_t *enc, TIM_HandleTypeDef *htim);
float Absolute_Encoder_GetPosition(Absolute_Encoder_t *enc);

// Quadrature Encoder Functions
void Quadrature_EncoderInit(Quadrature_Encoder_t *enc, TIM_HandleTypeDef *htim);
void Quadrature_EncoderUpdate(Quadrature_Encoder_t *enc);
int32_t Quadrature_GetPosition(Quadrature_Encoder_t *enc);

#endif // __REV_THROUGHBORE_ENCODER_H
