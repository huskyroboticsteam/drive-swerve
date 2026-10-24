#include "hall_encoders.h"


volatile HallState_t HallStates[HALL_COUNT] = {0};

HallConfig_t configs[HALL_COUNT] = {0};

void Hall_Init(HallId_t id, HallConfig_t config) {
    configs[id] = config;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    for (size_t i = 0; i < HALL_COUNT; ++i) {
        if (htim == configs[i].timer) {
            
        }
    }
}
