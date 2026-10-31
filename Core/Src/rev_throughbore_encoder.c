#include "rev_throughbore_encoder.h"


void Quadrature_EncoderInit(Encoder_t *enc, TIM_HandleTypeDef *htim) {
    enc->htim = htim;
    enc->position = 0; // change to PWM position?
    enc->last_counter = 0;

    // Start the Encoder interface in HAL
    HAL_TIM_Encoder_Start(enc->htim, TIM_CHANNEL_4);
}

// Call this more than once every 4 rotations
void Quadrature_EncoderUpdate(Encoder_t *enc) {
    // 1. Get the current 16-bit or 32-bit hardware counter value
    int16_t current_counter = __HAL_TIM_GET_COUNTER(enc->htim); //might be 16 bits not sure
    __HAL_TIM_SET_COUNTER(enc->htim,  0);

    // 2. Accumulate into a 32-bit signed integer
    enc->position += current_counter;

    // 3. Save the current counter for the next update
    enc->last_counter = current_counter;
}


int32_t Quadrature_GetPosition(Encoder_t *enc) {
	 return enc->position;
}

float PWM_To_Angle(uint32_t raw_duty, uint32_t min_duty, uint32_t max_duty, float min_angle, float max_angle) {

	// Constrain input to avoid out-of-bounds calculations due to signal noise
	if (raw_duty < min_duty) return min_angle;
	if (raw_duty > max_duty) return max_angle;

	 // Linear interpolation mapping
	float fraction = (float)(raw_duty - min_duty) / (float)(max_duty - min_duty);
	float angle = min_angle + (fraction * (max_angle - min_angle));

	return angle;
}

//void HAL_TIM_IC_CaptureCallback() {
//
//};
