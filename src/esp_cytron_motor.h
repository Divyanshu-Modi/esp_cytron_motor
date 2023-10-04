/*
 * Copyright (C) 2022, Divyanshu Modi <divyan.m05@gmail.com>
 */

#ifndef __ESP_CYTRON_MOTOR__
#define __ESP_CYTRON_MOTOR__

#include <Arduino.h>
#include <stdint.h>

enum esp_cytron_type {
	MOTOR_CHANNEL_0,
	MOTOR_CHANNEL_1,
	ENCODER_CHANNEL_0,
	ENCODER_CHANNEL_1,
};

class esp_cytron_motor {
	public:
		/*
		 * Function: Setup
		 * This will be used to setup the motors and their respective encoders
		 */
		void Setup(esp_cytron_type device_type, uint8_t pin1, uint8_t pin2);

		/*
		 * Function: Write
		 * This will be used to send pwm and state to the driver.
		 */
		void Write(esp_cytron_type device_type, uint8_t state, uint8_t pwm);

		/*
		 * Function: Read
		 * This will be used to read the encoder values.
		 */
		int Read(esp_cytron_type device_type);
};

#endif
