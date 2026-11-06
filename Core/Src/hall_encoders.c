#include "hall_encoders.h"

HallConfig_t Hall_configs[HALL_COUNT] = {0};

void Hall_Init(HallId_t id, HallConfig_t config) {
    Hall_configs[id] = config;
    HAL_TIMEx_HallSensor_Start(config.timer);
}

HallState_t Hall_Update(HallId_t id) {
    HallConfig_t config = Hall_configs[id];
    HallState_t result;
    result.a = HAL_GPIO_ReadPin(config.a_gpio, config.a_gpio_pin) == GPIO_PIN_SET;
    result.b = HAL_GPIO_ReadPin(config.b_gpio, config.b_gpio_pin) == GPIO_PIN_SET;
    result.c = HAL_GPIO_ReadPin(config.c_gpio, config.c_gpio_pin) == GPIO_PIN_SET;
    return result;
}
