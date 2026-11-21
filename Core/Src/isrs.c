#include "hall_encoders.h"
#include "commutation.h"
#include "rev_throughbore_encoder.h"
#include "drv8323.h"
#include "main.h"
#include <stdio.h>

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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  for (int i = 0; i < DRV8323_COUNT; i++) {
	  if (GPIO_Pin == Drv8323_configs[i].fault_pin) {
		  Commutation_SetDuty(i, 0);
		  uint16_t reg1 = Drv8323_GetRegister(i, DRV8323_REG_FAULT_STATUS_1);
		  uint16_t reg2 = Drv8323_GetRegister(i, DRV8323_REG_FAULT_STATUS_2);
		  uint8_t output[] = "(id: x) fault registers: xxxx xxxx\r\n";
		  sprintf((char*)output, "(id: %d) fault registers: %04x %04x\r\n", i, reg1, reg2);
		  HAL_UART_Transmit(&huart5, output, sizeof(output)-1, 100);
	  }
  }
}

