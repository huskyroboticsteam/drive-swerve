#include <current_sense.h>

static ADC_CurrentSense_t cs_table[CURRENT_SENSE_MAX];

static uint16_t adc_offset[PHASE_COUNT] = {2048, 2048, 2048};

static ADC_CurrentSense_t *find(const ADC_HandleTypeDef *hadc)
{
    for (uint32_t i = 0; i < CURRENT_SENSE_MAX; i++) {
        if (cs_table[i].hadc == hadc) return &cs_table[i];
    }
    return NULL;
}

static HAL_StatusTypeDef read_raw(ADC_HandleTypeDef *hadc, uint16_t raw[3]) {
    if (HAL_ADCEx_InjectedStart(hadc) != HAL_OK) return HAL_ERROR;
    if (HAL_ADCEx_InjectedPollForConversion(hadc, 2) != HAL_OK) return HAL_TIMEOUT;

    raw[0] = (uint16_t)HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
    raw[1] = (uint16_t)HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
    raw[2] = (uint16_t)HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_3);
    return HAL_OK;
}

// Registers hadc in table, calibrates ADC, measures offsets
// Call once per ADC after MX_ADCx_Init(), with that driver's gates disabled
HAL_StatusTypeDef CurrentSense_Init(ADC_HandleTypeDef *hadc) {

	if (hadc == NULL) return HAL_ERROR;
	if (find(hadc) != NULL) return HAL_ERROR; // already registered

	ADC_CurrentSense_t *cs = find(NULL);
	if (cs == NULL) return HAL_ERROR;

	uint32_t acc[3] = { 0 };
	uint16_t raw[3];

	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK) return HAL_ERROR;

	for (int n = 0; n < 64; n++) {
		if (read_raw(hadc, raw) != HAL_OK) return HAL_ERROR;
		for (int i = 0; i < PHASE_COUNT; i++) acc[i] += raw[i];
	}
	for (int i = 0; i < PHASE_COUNT; i++) {
		cs->offset[i] = (uint16_t)(acc[i] / 64u);
		cs->phase_currents[i] = 0.0f;
	}
	cs->hadc = hadc;
	return HAL_OK;
}

HAL_StatusTypeDef CurrentSense_Update(ADC_HandleTypeDef *hadc) {

	if (hadc == NULL) return HAL_ERROR;
	ADC_CurrentSense_t *cs = find(hadc);
	if (cs == NULL) return HAL_ERROR; // Not initialized

	uint16_t raw[PHASE_COUNT];
	HAL_StatusTypeDef st = read_raw(hadc, raw);
	if (st != HAL_OK) return st;

	for (int i = 0; i < PHASE_COUNT; i++)
		cs->phase_currents[i] = (float)((int32_t)adc_offset[i] - (int32_t)raw[i]) * A_PER_LSB;
	return HAL_OK;
}

// Latest updated currents for A, B, and C
float CurrentSense_GetPhaseCurrents(ADC_HandleTypeDef *hadc) {

	if (hadc == NULL) return NULL;
	ADC_CurrentSense_t *cs = find(hadc);
	return (cs != NULL) ? cs->phase_currents : NULL;
}


