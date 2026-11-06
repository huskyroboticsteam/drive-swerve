#include "hall_encoders.h"
#include "commutation.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    for (HallId_t i = 0; i < HALL_COUNT; ++i) {
        if (htim == Hall_configs[i].timer) {
            HallState_t state = Hall_Update(i);
            Commutation_UpdateGateDriver(i, state);
        }
    }
}
