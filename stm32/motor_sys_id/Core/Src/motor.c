# include <motor.h>

void motor_hardware_setup(motor *motor){
	// L298N motor driver setup
    // EN IN1 IN2 = PA8(PWM) PA9 PA10
	motor->driver.IN1_port = GPIOB;
    motor->driver.IN1_pin = GPIO_PIN_14;
	motor->driver.IN2_port = GPIOB;
    motor->driver.IN2_pin = GPIO_PIN_15;

	motor->driver.pwm_timer = &htim1;
	motor->driver.pwm_channel = TIM_CHANNEL_1;
	motor->driver.pwm_counter = 0;
	motor->driver.pwm_duty = 0;
	motor->driver.max_voltage = 12.24;

    // encoder parameter setup
    motor->control_frequency = 1000;
    motor->encoder.enc_timer = &htim2;
    motor->encoder.current_counter = 0;
    motor->encoder.previous_counter = 0;
    motor->encoder.count_per_rev = 500;
    motor->encoder.reduction_ratio = 16;

    HAL_TIM_Encoder_Start_IT(motor->encoder.enc_timer, TIM_CHANNEL_ALL);
}

void motor_driver_initialize(motor *motor){
	HAL_TIM_PWM_Start(motor->driver.pwm_timer, motor->driver.pwm_channel);
	// initialize as slow decay mode
	HAL_GPIO_WritePin(motor->driver.IN1_port, motor->driver.IN1_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motor->driver.IN2_port, motor->driver.IN2_pin, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(motor->driver.pwm_timer, motor->driver.pwm_channel,0);
}

void motor_duty_output(motor *motor, float duty_cycle, int dir){
	if (dir >= 0){
		HAL_GPIO_WritePin(motor->driver.IN1_port, motor->driver.IN1_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(motor->driver.IN2_port, motor->driver.IN2_pin, GPIO_PIN_RESET);
	}
	else {
		HAL_GPIO_WritePin(motor->driver.IN1_port, motor->driver.IN1_pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->driver.IN2_port, motor->driver.IN2_pin, GPIO_PIN_SET);
	}
	int timer_counter_period = motor->driver.pwm_timer->Instance->ARR + 1;
	motor->driver.pwm_counter = (float)timer_counter_period * duty_cycle;
    motor->driver.pwm_duty = duty_cycle;
	__HAL_TIM_SET_COMPARE(motor->driver.pwm_timer, motor->driver.pwm_channel, motor->driver.pwm_counter);
}

void motor_encoder_update(motor *motor){
    double rad_per_cnt = ((2*M_PI) / (motor->encoder.count_per_rev * motor->encoder.reduction_ratio * 4));

    motor->encoder.current_counter = motor->encoder.enc_timer->Instance->CNT;
    motor->encoder.delta_counter = motor->encoder.current_counter - motor->encoder.previous_counter;

    motor->angular_vel = motor->encoder.delta_counter * rad_per_cnt * motor->control_frequency;
    motor->encoder.previous_counter = motor->encoder.current_counter;
}

float voltage_to_dutycycle(motor *motor, float voltage){
	return voltage / motor->driver.max_voltage;
}

