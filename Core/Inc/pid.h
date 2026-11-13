
#ifndef DRIVE_SWERVE_CORE_SRC_PID_H_
#define DRIVE_SWERVE_CORE_SRC_PID_H_

// PID Structure Definition
typedef struct {
    float Kp, Ki, Kd;
    float setpoint;
    float integral;
    float last_error;
    float out_min, out_max;
} PID_Controller_t;

// PID Functions
void PID_Init(PID_Controller_t *pid, float kp, float ki, float kd, float min, float max);
float PID_Update(PID_Controller_t *pid, float target, float measured);

#endif /* DRIVE_SWERVE_CORE_SRC_PID_H_ */
