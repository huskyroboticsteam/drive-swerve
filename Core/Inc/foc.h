#ifndef __FOC_H
#define __FOC_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "pid.h"
#include "commutation.h"

#define FOC_VELOCITY_PID_FREQUENCY 32 // 20kHz / 32 = 625 Hz
#define FOC_POSITION_PID_FREQUENCY 64 // 20Khz / 64 = 312.5 Hz

#define FOC_MOTOR_COUNT 2

typedef struct {
    PID_Controller_t position_pid;
    PID_Controller_t velocity_pid;
    PID_Controller_t iq_pid;
    PID_Controller_t id_pid;
    uint32_t velocity_pid_counter; // whenever this hits 0, do iteration of pid loop
    uint32_t position_pid_counter; // reset to PID_FREQUENCY and decrement once each time iq_pid loop is done
    ADC_HandleTypeDef *current_sense;
    GateDriver_t gate_driver;
    bool position_control;
    bool enabled;
} FocMotor_t;

typedef struct {
    // everything is in terms of electrical revolutions
    float position_kp, position_ki, position_kd;
    float velocity_kp, velocity_ki, velocity_kd;
    float min_velocity, max_velocity;
    float iq_kp, iq_ki;
    float min_iq, max_iq;
    float id_kp, id_ki;
    ADC_HandleTypeDef *current_sense; // must be preconfigured
    GateDriver_t pwm_pins;
} FocMotor_Config_t;

extern volatile FocMotor_t Foc_motors[FOC_MOTOR_COUNT];

typedef size_t FocMotorId_t;

// Initializes a motor to run using FOC
// current sense should be configured before this
// Gate driver chip should be configured after this
void Foc_motor_init(FocMotorId_t id, FocMotor_Config_t config);

// Performs one step of FOC
// Intended to be done during timer interrupt on start of one of PWM phases
// Currently missing feed forward
void Foc_motor_step(FocMotorId_t id);

#endif
