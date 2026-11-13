#include "pid.h"

#define ENCODER_PPR 8192.0f // 2048 CPR * 4 edges
#define PID_DT 0.001f // 1 ms execution step

PID_Controller_t rotary_pid;

// Tracking state variables
int16_t last_encoder_count = 0;
float current_rpm = 0.0f;
float target_rpm = 1500.0f;    // Set your target speed here
int16_t current_pwm_output = 0;

void PID_Init(PID_Controller_t *pid, float kp, float ki, float kd, float min, float max) {
	pid->Kp = kp; pid->Ki = ki; pid->Kd = kd;
	pid->out_min = min; pid->out_max = max;
	pid->integral = 0.0f; pid->last_error = 0.0f;
}

float PID_Update(PID_Controller_t *pid, float target, float measured) {
    float error = target - measured;

    // Proportional term
    float P = pid->Kp * error;

    // Integral term (with anti-windup clamping)
    pid->integral += error * PID_DT;
    float I = pid->Ki * pid->integral;
    if (I > pid->out_max) I = pid->out_max;
    if (I < pid->out_min) I = pid->out_min;

    // Derivative term
    float D = pid->Kd * ((error - pid->last_error) / PID_DT);
    pid->last_error = error;

    // Sum total and constrain output limit
    float output = P + I + D;
    if (output > pid->out_max) output = pid->out_max;
    if (output < pid->out_min) output = pid->out_min;

    return output;
}
