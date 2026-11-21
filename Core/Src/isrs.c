#include "hall_encoders.h"
#include "commutation.h"
#include "rev_throughbore_encoder.h"
#include "drv8323.h"
#include "main.h"

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
  uint16_t reg1;
  uint16_t reg2;
  switch(GPIO_Pin) {
  	case nFAULT_D_Pin:
  	  Commutation_SetDuty(DRIVER_ID_DRIVE, 0);
  	  reg1 = Drv8323_GetRegister(DRV8323_ID_DRIVE, DRV8323_REG_FAULT_STATUS_1);
  	  reg2 = Drv8323_GetRegister(DRV8323_ID_DRIVE, DRV8323_REG_FAULT_STATUS_2);
	  break;
    case nFAULT_S_Pin:
      Commutation_SetDuty(DRIVER_ID_SWERVE, 0);
      reg1 = Drv8323_GetRegister(DRV8323_ID_SWERVE, DRV8323_REG_FAULT_STATUS_1);
      reg2 = Drv8323_GetRegister(DRV8323_ID_SWERVE, DRV8323_REG_FAULT_STATUS_2);
	  break;
    default:

  }
}

