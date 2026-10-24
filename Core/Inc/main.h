/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Index_Swer_Encoder_Pin GPIO_PIN_13
#define Index_Swer_Encoder_GPIO_Port GPIOC
#define ENABLE_S_Pin GPIO_PIN_14
#define ENABLE_S_GPIO_Port GPIOC
#define ENABLE_D_Pin GPIO_PIN_15
#define ENABLE_D_GPIO_Port GPIOC
#define CS_D_Pin GPIO_PIN_1
#define CS_D_GPIO_Port GPIOF
#define SOA_D_Pin GPIO_PIN_0
#define SOA_D_GPIO_Port GPIOC
#define SOB_D_Pin GPIO_PIN_1
#define SOB_D_GPIO_Port GPIOC
#define SOC_D_Pin GPIO_PIN_2
#define SOC_D_GPIO_Port GPIOC
#define CS_S_Pin GPIO_PIN_3
#define CS_S_GPIO_Port GPIOC
#define SOA_S_Pin GPIO_PIN_0
#define SOA_S_GPIO_Port GPIOA
#define SOB_S_Pin GPIO_PIN_1
#define SOB_S_GPIO_Port GPIOA
#define SOC_S_Pin GPIO_PIN_2
#define SOC_S_GPIO_Port GPIOA
#define Temp_Swer_Pin GPIO_PIN_4
#define Temp_Swer_GPIO_Port GPIOA
#define SCLK_Pin GPIO_PIN_5
#define SCLK_GPIO_Port GPIOA
#define SDO_Pin GPIO_PIN_6
#define SDO_GPIO_Port GPIOA
#define SDI_Pin GPIO_PIN_7
#define SDI_GPIO_Port GPIOA
#define SW0_Pin GPIO_PIN_4
#define SW0_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_5
#define SW1_GPIO_Port GPIOC
#define SW2_Pin GPIO_PIN_0
#define SW2_GPIO_Port GPIOB
#define INLC_D_Pin GPIO_PIN_1
#define INLC_D_GPIO_Port GPIOB
#define SW3_Pin GPIO_PIN_7
#define SW3_GPIO_Port GPIOE
#define CAN_LED_Pin GPIO_PIN_8
#define CAN_LED_GPIO_Port GPIOE
#define DBG_LED_Pin GPIO_PIN_9
#define DBG_LED_GPIO_Port GPIOE
#define ERR_LED_Pin GPIO_PIN_10
#define ERR_LED_GPIO_Port GPIOE
#define Temp_Drive_Pin GPIO_PIN_11
#define Temp_Drive_GPIO_Port GPIOE
#define INHA_D_Pin GPIO_PIN_13
#define INHA_D_GPIO_Port GPIOE
#define INHB_D_Pin GPIO_PIN_14
#define INHB_D_GPIO_Port GPIOE
#define INHC_D_Pin GPIO_PIN_15
#define INHC_D_GPIO_Port GPIOE
#define nFAULT_S_Pin GPIO_PIN_12
#define nFAULT_S_GPIO_Port GPIOB
#define INLA_S_Pin GPIO_PIN_13
#define INLA_S_GPIO_Port GPIOB
#define INLB_S_Pin GPIO_PIN_14
#define INLB_S_GPIO_Port GPIOB
#define INLC_S_Pin GPIO_PIN_15
#define INLC_S_GPIO_Port GPIOB
#define CAL_S_Pin GPIO_PIN_9
#define CAL_S_GPIO_Port GPIOD
#define CAL_D_Pin GPIO_PIN_10
#define CAL_D_GPIO_Port GPIOD
#define Hall_A_Drive_Pin GPIO_PIN_6
#define Hall_A_Drive_GPIO_Port GPIOC
#define Hall_B_Drive_Pin GPIO_PIN_7
#define Hall_B_Drive_GPIO_Port GPIOC
#define Hall_C_Drive_Pin GPIO_PIN_8
#define Hall_C_Drive_GPIO_Port GPIOC
#define nFAULT_D_Pin GPIO_PIN_9
#define nFAULT_D_GPIO_Port GPIOC
#define Hall_A_Swer_Pin GPIO_PIN_8
#define Hall_A_Swer_GPIO_Port GPIOA
#define Hall_B_Swer_Pin GPIO_PIN_9
#define Hall_B_Swer_GPIO_Port GPIOA
#define Hall_C_Swer_Pin GPIO_PIN_10
#define Hall_C_Swer_GPIO_Port GPIOA
#define INLA_D_Pin GPIO_PIN_10
#define INLA_D_GPIO_Port GPIOC
#define INLB_D_Pin GPIO_PIN_11
#define INLB_D_GPIO_Port GPIOC
#define VCP_TX_Pin GPIO_PIN_12
#define VCP_TX_GPIO_Port GPIOC
#define ABS_Swer_Encoder_Pin GPIO_PIN_1
#define ABS_Swer_Encoder_GPIO_Port GPIOD
#define VCP_RX_Pin GPIO_PIN_2
#define VCP_RX_GPIO_Port GPIOD
#define INHA_S_Pin GPIO_PIN_4
#define INHA_S_GPIO_Port GPIOB
#define INHB_S_Pin GPIO_PIN_5
#define INHB_S_GPIO_Port GPIOB
#define A_Swerv_Encoder_Pin GPIO_PIN_6
#define A_Swerv_Encoder_GPIO_Port GPIOB
#define B_Swerv_Encoder_Pin GPIO_PIN_7
#define B_Swerv_Encoder_GPIO_Port GPIOB
#define INHC_S_Pin GPIO_PIN_9
#define INHC_S_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
