#include "foc.h"
#include "clarke_park_transforms.h"
#include "current_sense.h"
#include <math.h>

volatile FocMotor_t Foc_motors[FOC_MOTOR_COUNT] = {0};

void Foc_motor_init(FocMotorId_t id, FocMotor_Config_t config) {
    FocMotor_t result;
    PID_Init(&result.position_pid, config.position_kp, config.position_ki, config.position_kd,
            config.min_velocity, config.max_velocity);
    PID_Init(&result.velocity_pid, config.velocity_kp, config.velocity_ki, config.velocity_kd,
            config.min_iq, config.max_iq);
    PID_Init(&result.iq_pid, config.iq_kp, config.iq_ki, 0, 0, 1);
    PID_Init(&result.id_pid, config.id_kp, config.id_ki, 0, 0, 1);
    // Initialize setpoints to 0
    result.position_pid.setpoint = 0;
    result.velocity_pid.setpoint = 0;
    result.iq_pid.setpoint = 0;
    result.id_pid.setpoint = 0;
    // Start pid counters at 0 (force first iteration to do pid)
    result.velocity_pid_counter = 0;
    result.position_pid_counter = 0;
    
    result.current_sense = config.current_sense;
    result.gate_driver = config.pwm_pins;
    result.position_control = false;
    result.enabled = false;
    Foc_motors[id] = result;
}

// note that NaN doesn't behave correctly here
static float clamp(float x, float low, float high) {
    if (x <= low) return low;
    if (x >= high) return high;
    return x;
}

void Foc_motor_step(FocMotorId_t id) {
    // note that volatile attribute is discarded here, which is fine in theory
    FocMotor_t *config = (FocMotor_t *)&Foc_motors[id];
    if (!config->enabled) {
        // Set state to hi-z
        Commutation_DrivePin(config->gate_driver.a, GATE_HIZ, 0);
        Commutation_DrivePin(config->gate_driver.b, GATE_HIZ, 0);
        Commutation_DrivePin(config->gate_driver.c, GATE_HIZ, 0);
        return;
    }
    // Current Sense
    float *currents = (float *)CurrentSense_GetPhaseCurrents(config->current_sense);

    // TODO: Read encoder
    float position_estimate = 0; // electrical rev
    float velocity_estimate = 0; // electrical rev / s

    // Outer pid
    if (config->position_control && --config->position_pid_counter == 0) {
        config->position_pid_counter = FOC_POSITION_PID_FREQUENCY;
        float new_vel = PID_Update(&config->position_pid, position_estimate);
        config->velocity_pid.setpoint = new_vel;
    }
    if (--config->velocity_pid_counter == 0) {
        config->velocity_pid_counter = FOC_VELOCITY_PID_FREQUENCY;
        float new_iq = PID_Update(&config->velocity_pid, velocity_estimate);
        config->iq_pid.setpoint = new_iq;
    }
    
    // Clarke + park
    position_estimate -= truncf(position_estimate); // remove integer component
    clarkeCurrents_t clarke = clarkeTransform(currents[0], currents[1], currents[2]);
    parkCurrents_t park = parkTransform(clarke.a, clarke.b, position_estimate);

    // Iq + Id PI
    config->id_pid.setpoint = 0; // these are done to hopefully improve inlined behavior
    config->id_pid.Kd = 0; // derivative not needed
    config->iq_pid.Kd = 0;
    float d = PID_Update(&config->id_pid, park.d);
    float q = PID_Update(&config->iq_pid, park.q);

    // inverse park + clarke
    clarkeCurrents_t clarke_duties = inversePark(d, q, position_estimate);
    rawCurrents_t duties = inverseClarke(clarke_duties.a, clarke_duties.b);

    // svpwm
    duties.a = clamp(duties.a, 0, 0.95);
    duties.b = clamp(duties.b, 0, 0.95);
    duties.c = clamp(duties.c, 0, 0.95);
    Commutation_DrivePin(config->gate_driver.a, GATE_PWM, duties.a);
    Commutation_DrivePin(config->gate_driver.a, GATE_PWM, duties.b);
    Commutation_DrivePin(config->gate_driver.a, GATE_PWM, duties.c);
}
