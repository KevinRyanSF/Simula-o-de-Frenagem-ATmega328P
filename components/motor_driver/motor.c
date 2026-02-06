/*
 * motor.c
 *
 * Created: 05/02/2026 21:58:38
 *  Author: kevin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "motor.h"

// Ponte H
#define EN_PIN   PD3   // OC2B
#define IN1_PIN  PD4
#define IN2_PIN  PD5

// Controles
#define DIR_PIN  PC1   // switch p/ GND (ré)
#define ADC_CH   0     // ADC0 (PC0)

static uint8_t pwm_limit = 255;


static uint16_t adc_read(uint8_t ch)
{
	ADMUX = (1 << REFS0) | (ch & 0x0F); // AVcc
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void motor_init(void)
{
	// Ponte H
	DDRD |= (1 << EN_PIN) | (1 << IN1_PIN) | (1 << IN2_PIN);

	// Direção (pull-up)
	DDRC &= ~(1 << DIR_PIN);
	PORTC |= (1 << DIR_PIN);

	// ADC
	DDRC &= ~(1 << PC0);
	PORTC &= ~(1 << PC0);

	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) |
	(1 << ADPS1) | (1 << ADPS0);

	// Timer2 ? Fast PWM OC2B
	TCCR2A = (1 << WGM21) | (1 << WGM20) | (1 << COM2B1);
	TCCR2B = (1 << CS22); // prescaler 64 ? ~1 kHz

	OCR2B = 0;
}

void motor_update(void)
{
	uint16_t adc = adc_read(ADC_CH);
	uint8_t pwm = adc >> 2;

	// zona morta
	if (pwm > 0 && pwm < 50)
	pwm = 50;

	// aplica limite do freio inteligente
	if (pwm > pwm_limit)
	pwm = pwm_limit;

	// Direção
	if (!(PINC & (1 << DIR_PIN)))
	{
		// Ré
		PORTD |= (1 << IN1_PIN);
		PORTD &= ~(1 << IN2_PIN);
	}
	else
	{
		// Frente
		PORTD &= ~(1 << IN1_PIN);
		PORTD |= (1 << IN2_PIN);
	}

	OCR2B = pwm;
}



void motor_brake(void)
{
	// Freio ativo (IN1 e IN2 LOW)
	PORTD &= ~(1 << IN1_PIN);
	PORTD &= ~(1 << IN2_PIN);
	OCR2B = 0;
}

void motor_set_pwm_limit(uint8_t limit)
{
	pwm_limit = limit;
}


