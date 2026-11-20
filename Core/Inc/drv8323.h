#ifndef __DRV8323_H
#define __DRV8323_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32g4xx_hal.h"

// Adapted from Becca's driver

#define DRV8323_COUNT 2

#define DRV8323_ID_DRIVE 0
#define DRV8323_ID_SWERVE 1

typedef struct {
    SPI_HandleTypeDef *spi;

    GPIO_TypeDef *cs_gpio;
    uint16_t cs_pin;

    GPIO_TypeDef *enable_gpio;
    uint16_t enable_pin;

    GPIO_TypeDef *fault_gpio;
    uint16_t fault_pin;
} Drv8323_t;

// Registers

// Read only
#define DRV8323_REG_FAULT_STATUS_1 0
typedef union {
    struct {
        uint16_t vds_lc : 1; // Indicates VDS overcurrent fault on the C low-side MOSFET
        uint16_t vds_hc : 1; // Indicates VDS overcurrent fault on the C high-side MOSFET
        uint16_t vds_lb : 1; // Indicates VDS overcurrent fault on the B low-side MOSFET
        uint16_t vds_hb : 1; // Indicates VDS overcurrent fault on the B high-side MOSFET
        uint16_t vds_la : 1; // Indicates VDS overcurrent fault on the A low-side MOSFET
        uint16_t vds_ha : 1; // Indicates VDS overcurrent fault on the A high-side MOSFET
        uint16_t otsd : 1; // Indicates overtemperature shutdown
        uint16_t uvlo : 1; // Indicates undervoltage lockout fault condition
        uint16_t gdf : 1; // Indicates gate drive fault condition
        uint16_t vds_ocp : 1; // Indicates VDS monitor overcurrent fault condition
        uint16_t fault : 1; // Logic OR of FAULT status registers. Mirrors nFAULT pin.
    };
    uint16_t full_value;
} Drv8323_Fault_Status_1_t;

// Read only
#define DRV8323_REG_FAULT_STATUS_2 1
typedef union {
    struct {
        uint16_t vgs_lc : 1; // Indicates gate drive fault on the C low-side MOSFET
        uint16_t vgs_hc : 1; // Indicates gate drive fault on the C high-side MOSFET
        uint16_t vgs_lb : 1; // Indicates gate drive fault on the B low-side MOSFET
        uint16_t vgs_hb : 1; // Indicates gate drive fault on the B high-side MOSFET
        uint16_t vgs_la : 1; // Indicates gate drive fault on the A low-side MOSFET
        uint16_t vgs_ha : 1; // Indicates gate drive fault on the A high-side MOSFET
        uint16_t cpuv : 1; // Indicates charge pump undervoltage fault condition
        uint16_t otw : 1; // Indicates overtemperature warning
        uint16_t sc_oc : 1; // Indicates overcurrent on phase C sense amplifier
        uint16_t sb_oc : 1; // Indicates overcurrent on phase B sense amplifier
        uint16_t sa_oc : 1; // Indicates overcurrent on phase A sense amplifier
    };
    uint16_t full_value;
} Drv8323_Fault_Status_2_t;

// Read/Write
#define DRV8323_REG_DRIVER_CONTROL 2
#define DRV8323_PWM_MODE_6x 0
#define DRV8323_PWM_MODE_3x 1
#define DRV8323_PWM_MODE_1x 2
#define DRV8323_PWM_MODE_INDEPENDENT 3
typedef union {
    struct {
        uint16_t clf_flt : 1; // Write a 1 to this bit to clear latched fault bits
        uint16_t brake : 1; // Write a 1 to this bit to turn on all three low-side MOSFETs in 1x PWM mode.
        uint16_t coast : 1; // Write a 1 to this bit to put all MOSFETs in the Hi-Z state
        uint16_t x1pwm_dir : 1; // In 1x PWM mode this bit is ORed with the INHC (DIR) input
        uint16_t x1pwm_com : 1; // 0b = 1x PWM mode uses synchronous rectification, 1b = asynchronous rectification
        uint16_t x1pwm_mode : 2; // Use DRV8323_PWM_MODE_x macros
        uint16_t otw_rep : 1; // Controls whether OTW is reported in the fault bits and nFAULT pin
        uint16_t dis_gdf : 1; // Controls whether gate driver fault is enabled
        uint16_t dis_cpuv : 1; // Controls whether charge pump uvlo fault is enabled
        uint16_t _reserved : 1;
    };
    uint16_t full_value;
} Drv8323_Driver_Control_t;

// Read/Write
#define DRV8323_REG_GATE_DRIVE_HS 3
typedef union {
    struct {
        uint16_t idriven_hs : 4; // negative current through high side gate driver, see datasheet for numerical mapping
        uint16_t idrivep_hs : 4; // positive current through high side gate driver, see datasheet for numerical mapping
        uint16_t lock : 3; // write 110b to lock settings, 011b to unlock
    };
    uint16_t full_value;
} Drv8323_Gate_Drive_HS_t;

// Read/Write
#define DRV8323_REG_GATE_DRIVE_LS 4
typedef union {
    struct {
        uint16_t idriven_ls : 4; // negative current through low side gate driver, see datasheet for numerical mapping
        uint16_t idrivep_ls : 4; // positive current through low side gate driver, see datasheet for numerical mapping
        uint16_t tdrive : 2; // max drive time
        uint16_t cbc : 1; // cycle by cycle operation, causes certain faults to be automatically reset with pwm input
    };
    uint16_t full_value;
} Drv8323_GateDrive_LS_t;

// Read/Write
#define DRV8323_REG_OCP_CONTROL 5
typedef union {
    struct {
        uint16_t vds_lvl : 4; // see datasheet for numerical mapping
        uint16_t ocp_deg : 2; // Overcurrent deglitch time
        uint16_t ocp_mode : 2; // Type of fault triggered on overcurrent
        uint16_t dead_time : 2; // Amount of dead time inserted into complementary pwm
        uint16_t tretry : 1; // 0 - retry time of 4ms, 1 - retry time of 50us
    };
    uint16_t full_value;
} Drv8323_OCP_Control_t;

// Read/Write
#define DRV8323_REG_CSA_CONTROL 6
typedef union {
    struct {
        uint16_t sen_lvl : 2; // Sense OCP level, see datasheet for values
        uint16_t csa_cal_c : 1; // 1 = short inputs to CSA C for offset calibration
        uint16_t csa_cal_b : 1; // 1 = short inputs to CSA B for offset calibration
        uint16_t csa_cal_a : 1; // 1 = short inputs to CSA A for offset calibration
        uint16_t dis_sen : 1; // 1 = disable sense overcurrent fault
        uint16_t csa_gain : 2; // see datasheet for values
        uint16_t ls_ref : 1;
        uint16_t vref_div : 1; // 1 = CSA ref voltage is divided by 2
        uint16_t csa_fet : 1; // changes what the CSA positive input is
    };
    uint16_t full_value;
} Drv8323_CSA_Control_t;

typedef size_t Drv8323Id_t;

extern Drv8323_t Drv8323_configs[DRV8323_COUNT];

void Drv8323_Init(Drv8323Id_t id, Drv8323_t config);

void Drv8323_SetEnable(Drv8323Id_t id, bool enable);

void Drv8323_SetRegister(Drv8323Id_t id, uint8_t reg, uint16_t value);

uint16_t Drv8323_GetRegister(Drv8323Id_t id, uint8_t reg);

#endif
