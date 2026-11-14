#include "hall_encoders.h"
#include "commutation.h"
#include "rev_throughbore_encoder.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

    for (HallId_t i = 0; i < HALL_COUNT; ++i) {
        if (htim == Hall_configs[i].timer) {
            HallState_t state = Hall_Update(i);
            Commutation_UpdateGateDriver(i, state);
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	for (int i = 0; i < QUADRATURE_COUNT; ++i) {
		if (htim == tim_list[i]->htim) {
			Quadrature_HAL_TIM_PeriodElapsedCallback(i);
		}
	}
//	if (htim->Instance == TIM4) {
//		Quadrature_HAL_TIM_PeriodElapsedCallback(0);
//		return;
//	}
}
