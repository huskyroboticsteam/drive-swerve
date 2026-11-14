/*
 * clarke_park_transforms.h
 */

#include <math.h>
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_cordic.h"

#ifndef DRIVE_SWERVE_CORE_INC_CLARKE_PARK_TRANSFORMS_H_
#define DRIVE_SWERVE_CORE_INC_CLARKE_PARK_TRANSFORMS_H_

typedef struct {
	float a;
	float b;
	float c;
} rawCurrents_t;

typedef struct {
	float a;
	float b;
} clarkeCurrents_t;

typedef struct {
	float d;
	float q;
} parkCurrents_t;

inline static clarkeCurrents_t clarkeTransform(float a, float b, float c) {
	return (clarkeCurrents_t){a, (a + 2 * b)*(1/sqrtf(3))};
}

inline static rawCurrents_t inverseClarke(float a, float b) {
	float ib = -0.5f * a + 0.5f * sqrtf(3) * b;
	return (rawCurrents_t){a, ib, -(a+ib)};
}

inline static parkCurrents_t parkTransform(float a, float b, float theta) {
	LL_CORDIC_WriteData(CORDIC, theta);
	float cos = (float) LL_CORDIC_ReadData(CORDIC);
	float sin = (float) LL_CORDIC_ReadData(CORDIC);
	return (parkCurrents_t){a * cos + b * sin, -a * sin + b * cos};
}

inline static clarkeCurrents_t inversePark(float d, float q, float theta) {
	LL_CORDIC_WriteData(CORDIC, theta);
	float cos = (float) LL_CORDIC_ReadData(CORDIC);
	float sin = (float) LL_CORDIC_ReadData(CORDIC);
	return (clarkeCurrents_t){d * cos - q * sin, d * sin + q * cos};
}

#endif /* DRIVE_SWERVE_CORE_INC_CLARKE_PARK_TRANSFORMS_H_ */
