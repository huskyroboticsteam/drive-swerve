#include "rev_throughbore_encoder.h"

// Absolute Encoder Constants

Quadrature_Encoder_t *tim_list[1];

const static int16_t RISING_EDGE_CHANNEL = TIM_CHANNEL_1;
const static int16_t FALLING_EDGE_CHANNEL = TIM_CHANNEL_2;

void Quadrature_EncoderInit(Quadrature_Encoder_t *enc, TIM_HandleTypeDef *htim) {
    enc->htim = htim;
    enc->position = 0; // change to PWM position?
    enc->last_counter = 0;
    enc->last_time_read = 0;
    enc->velocity = 0.0f;
    // Start the Encoder interface in HAL
    HAL_TIM_Encoder_Start(enc->htim, TIM_CHANNEL_4);
}

// Call this more than once every 4 rotations
void Quadrature_EncoderUpdate(Quadrature_Encoder_t *enc) {

    // 1. Get the current 16-bit or 32-bit hardware counter value
    int16_t current_counter = __HAL_TIM_GET_COUNTER(enc->htim); //might be 16 bits not sure
    uint32_t current_tick = HAL_GetTick();

    __HAL_TIM_SET_COUNTER(enc->htim,  0);

    int32_t delta_time = current_tick - enc->last_time_read;

    // 2. Accumulate into a 32-bit signed integer
    enc->position += current_counter;

    // 3. Save the current counter for the next update
    enc->last_counter = current_counter;
    enc->last_time_read = current_tick;
    enc->delta_time = delta_time;
}

float Quadrature_GetVelocity(Quadrature_Encoder_t *enc) {
	float velocity = (float) (enc->last_counter) / (float) (enc->delta_time);
	return velocity;
}


int32_t Quadrature_GetPosition(Quadrature_Encoder_t *enc) {
	 return enc->position;
}

void Absolute_EncoderInit(Absolute_Encoder_t *enc, TIM_HandleTypeDef *htim) {
    enc->htim = htim;
    HAL_TIM_PWM_Start(enc->htim, RISING_EDGE_CHANNEL); // total period (rising edge) channel
    HAL_TIM_PWM_Start(enc->htim, FALLING_EDGE_CHANNEL); //
}

// Call this more than once every 4 rotations, returns angle in degrees
float Absolute_Encoder_GetPosition(Absolute_Encoder_t *enc) {

	int32_t period = HAL_TIM_ReadCapturedValue(enc->htim, RISING_EDGE_CHANNEL);
	int32_t pulse = HAL_TIM_ReadCapturedValue(enc->htim, FALLING_EDGE_CHANNEL);
	float duty = (float)pulse / (float)period;

    return duty * 360.0f;
}

void Quadrature_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	Quadrature_EncoderUpdate(&tim_list[0]);
	float current_velocity = Quadrature_GetVelocity(&tim_list[0]);
	&tim_list[0]->velocity = current_velocity;
}
