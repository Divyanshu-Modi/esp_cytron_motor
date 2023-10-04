/*
 * Copyright (C) 2022, Divyanshu Modi <divyan.m05@gmail.com>
 */
#include <esp_cytron_motor.h>

/*
 * TODO: Allow user to manipulate the frequency and resolution
 */
#define LEDC_FREQ 3000
#define LEDC_RES 8

typedef struct esp_cytron {
	uint8_t channel;
	uint8_t dir_pin;
	uint8_t pwm_pin;
} esp_cytron;

typedef struct esp_cytron_encoder {
	uint8_t channel_0;
	uint8_t channel_1;
	volatile int count;
} esp_cytron_encoder;

esp_cytron mot[2] = { 
	{ 0, 255, 255 },
	{ 1, 255, 255 },
};

esp_cytron_encoder encoder[2] = {
	{ 255, 255, 0 },
	{ 255, 255, 0 }, 
};

static void esp_cytron_encoder_isr0(void)
{
	uint8_t chanA = digitalRead(encoder[0].channel_0);
	uint8_t chanB = digitalRead(encoder[0].channel_1);

	if ((chanA == HIGH) && (chanB == LOW))
		encoder[0].count++;
	else
		encoder[0].count--;
}

static void esp_cytron_encoder_isr1(void)
{
	uint8_t chanA = digitalRead(encoder[1].channel_0);
	uint8_t chanB = digitalRead(encoder[1].channel_1);

	if ((chanA == HIGH) && (chanB == LOW))
		encoder[1].count++;
	else
		encoder[1].count--;
}

void esp_cytron_motor :: Setup(esp_cytron_type device_type, uint8_t pin1, uint8_t pin2)
{
	switch (device_type) {
	case MOTOR_CHANNEL_0: {
		if (mot[0].dir_pin == 255)
			mot[0].dir_pin = pin1;

		if (mot[0].pwm_pin == 255) {
			ledcSetup(mot[0].channel, LEDC_RES, LEDC_FREQ);
			ledcAttachPin(pin2, mot[0].channel);
			mot[0].pwm_pin = pin2;
		}
		break;
	}
	case MOTOR_CHANNEL_1: {
		if (mot[1].dir_pin == 255)
			mot[1].dir_pin = pin1;

		if (mot[1].pwm_pin == 255) {
			ledcSetup(mot[1].channel, LEDC_RES, LEDC_FREQ);
			ledcAttachPin(pin2, mot[1].channel);
			mot[1].pwm_pin = pin2;
		}
		break;
	}
	case ENCODER_CHANNEL_0: {
		if (encoder[0].channel_0 == 255) {
			attachInterrupt(digitalPinToInterrupt(pin1), esp_cytron_encoder_isr0, CHANGE);
			encoder[0].channel_0 = pin1;
		}

		if (encoder[0].channel_1 == 255)
			encoder[0].channel_1 = pin2;

		break;
	}
	case ENCODER_CHANNEL_1: {
		if (encoder[1].channel_0 == 255) {
			encoder[1].channel_0 = pin1;
			attachInterrupt(digitalPinToInterrupt(pin1), esp_cytron_encoder_isr1, CHANGE);
		}

		if (encoder[1].channel_1 == 255)
			encoder[1].channel_1 = pin2;

		break;
	}
	default:
		break;
	}
}

void esp_cytron_motor :: Write(esp_cytron_type device_type, uint8_t state, uint8_t pwm)
{
	switch (device_type) {
	case MOTOR_CHANNEL_0: {
		if ((mot[0].dir_pin == 255) || (mot[0].pwm_pin == 255))
			break;

		digitalWrite(mot[0].dir_pin, state);
		ledcWrite(mot[0].pwm_pin, pwm);
		break;
	}
	case MOTOR_CHANNEL_1: {
		if ((mot[1].dir_pin == 255) || (mot[1].pwm_pin == 255))
			break;

		digitalWrite(mot[1].dir_pin, state);
		ledcWrite(mot[1].pwm_pin, pwm);
		break;
	}
	default:
		break;
	}
}

int esp_cytron_motor :: Read(esp_cytron_type device_type)
{
	switch (device_type) {
	case ENCODER_CHANNEL_0:
		return encoder[0].count;
	case ENCODER_CHANNEL_1:
		return encoder[1].count;
	default:
		break;
	}

	return 0;
}

