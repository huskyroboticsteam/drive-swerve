#include <assert.h>
#include "drv8323.h"

Drv8323_t Drv8323_configs[DRV8323_COUNT];

void Drv8323_Init(Drv8323Id_t id, Drv8323_t config) {
    Drv8323_configs[id] = config;
    // Config mostly copied from Becca's code
    Drv8323_SetRegister(id, DRV8323_REG_DRIVER_CONTROL, (Drv8323_Driver_Control_t){
            .otw_rep = 1,
            .dis_gdf = 1,
            .dis_cpuv = 1
    }.full_value);
    Drv8323_SetRegister(id, DRV8323_REG_GATE_DRIVE_HS, (Drv8323_Gate_Drive_HS_t){
            .idriven_hs = 3,
            .idrivep_hs = 3,
            .lock = 3
    }.full_value);
    Drv8323_SetRegister(id, DRV8323_REG_GATE_DRIVE_LS, (Drv8323_Gate_Drive_LS_t){
            .idriven_ls = 3,
            .idrivep_ls = 3,
            .tdrive = 2, // 2000ns
    }.full_value);
    Drv8323_SetRegister(id, DRV8323_REG_OCP_CONTROL, (Drv8323_OCP_Control_t){
            .vds_lvl = 0,
            .ocp_deg = 2, // 4us deglitch
            .ocp_mode = 1, // auto-retry on overcurrent
            .dead_time = 1, // 100ns
    }.full_value);
    Drv8323_SetRegister(id, DRV8323_REG_CSA_CONTROL, (Drv8323_CSA_Control_t){
            .sen_lvl = 3, // 1V over-current sense level
            .csa_gain = 2, // 20
            .vref_div = 1 // divide vref by 2
    }.full_value);
}

void Drv8323_SetEnable(Drv8323Id_t id, bool enable) {
    Drv8323_t *config = &Drv8323_configs[id];
    HAL_GPIO_WritePin(config->enable_gpio, config->enable_pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Drv8323_SetRegister(Drv8323Id_t id, uint8_t reg, uint16_t value) {
    uint16_t payload = (reg & 0x7) << 10 | (value & 0x3FF); 
    Drv8323_t *config = &Drv8323_configs[id];

    HAL_GPIO_WritePin(config->cs_gpio, config->cs_pin, GPIO_PIN_SET);
    // Possibly need delay for lead time
    HAL_StatusTypeDef ret = HAL_SPI_Transmit(
        config->spi,
        (uint8_t *)&payload,
        1,
        1000
    );
    assert(ret == HAL_OK);
    // Possibly need delay for trail time
    HAL_GPIO_WritePin(config->cs_gpio, config->cs_pin, GPIO_PIN_RESET);

    // need delay between frames
    // Maybe insert at beginning based on state?
    HAL_Delay(1);
}

uint16_t Drv8323_GetRegister(Drv8323Id_t id, uint8_t reg) {
    uint16_t payload = (reg & 0x7) << 10 | 1 << 15;
    uint16_t data = 0;
    Drv8323_t *config = &Drv8323_configs[id];

    HAL_GPIO_WritePin(config->cs_gpio, config->cs_pin, GPIO_PIN_SET);
    // Possibly need delay for lead time
    HAL_StatusTypeDef ret = HAL_SPI_TransmitReceive(
        config->spi,
        (uint8_t *)&payload,
        (uint8_t *)&data,
        1,
        1000
    );
    assert(ret == HAL_OK);
    // Possibly need delay 
    HAL_GPIO_WritePin(config->cs_gpio, config->cs_pin, GPIO_PIN_RESET);

    HAL_Delay(1);
    return data;
}
