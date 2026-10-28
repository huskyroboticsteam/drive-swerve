#include "hall_encoders.h"


volatile HallState_t Hall_States[HALL_COUNT] = {0};

HallConfig_t configs[HALL_COUNT] = {0};

void Hall_Init(HallId_t id, HallConfig_t config) {
    configs[id] = config;
}

void Hall_Update(HallId_t id) {
    HallConfig_t config = configs[id];
    Hall_States[id].a = HAL_GPIO_ReadPin(config.a_gpio, config.a_gpio_pin) == GPIO_PIN_SET;
    Hall_States[id].b = HAL_GPIO_ReadPin(config.b_gpio, config.b_gpio_pin) == GPIO_PIN_SET;
    Hall_States[id].c = HAL_GPIO_ReadPin(config.c_gpio, config.c_gpio_pin) == GPIO_PIN_SET;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    for (HallId_t i = 0; i < HALL_COUNT; ++i) {
        if (htim == configs[i].timer) {
            Hall_Update(i);            
        }
    }
}
