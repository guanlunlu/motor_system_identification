#ifndef __robot_H
#define __robot_H

# include <main.h>
# include <math.h>

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern float random_input[];


typedef struct driver_hardware{
    GPIO_TypeDef * IN1_port;
    uint16_t IN1_pin;

    GPIO_TypeDef * IN2_port;
    uint16_t IN2_pin;

    GPIO_TypeDef * EN_port;
    uint16_t EN_pin;

    TIM_HandleTypeDef * pwm_timer;
    unsigned int pwm_channel;
    float pwm_duty;
    float pwm_counter;
    float max_voltage;
}Driver;

typedef struct encoder_param{
    int count_per_rev;
    int reduction_ratio;

    int previous_counter;
    int current_counter;
    int delta_counter;

    TIM_HandleTypeDef * enc_timer;
}Encoder;

typedef struct motor{
	Driver driver;
	Encoder encoder;
    
    int control_frequency;
	double angular_vel;

}motor;

void motor_hardware_setup(motor *motor);

void motor_encoder_update(motor *motor);

void motor_driver_initialize(motor *motor);

void motor_duty_output(motor *motor, float duty_cycle, int dir);

float voltage_to_dutycycle(motor *motor, float voltage);


#endif
