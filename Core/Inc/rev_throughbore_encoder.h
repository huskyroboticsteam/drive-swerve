#ifndef __REV_THROUGHBORE_ENCODER_H
#define __REV_THROUGHBORE_ENCODER_H

#include "stm32g4xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim; // Pointer to the timer handle
    int32_t position;        // Total accumulated position with overflow
    uint32_t last_counter;   // Previous hardware counter value
} Encoder_t;

float PWM_To_Angle(uint32_t raw_duty, uint32_t min_duty, uint32_t max_duty, float min_angle, float max_angle);

void Quadrature_EncoderInit(Encoder_t *enc, TIM_HandleTypeDef *htim);
void Quadrature_EncoderUpdate(Encoder_t *enc);
int32_t Quadrature_GetPosition(Encoder_t *enc);

int abs_pos();
int incr_pos();

#endif // __REV_THROUGHBORE_ENCODER_H
