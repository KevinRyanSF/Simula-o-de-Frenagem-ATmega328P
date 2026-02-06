/*
 * buzzer.c
 *
 * Created: 05/02/2026 22:08:58
 *  Author: kevin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buzzer.h"

#define BUZZER_DDR   DDRD
#define BUZZER_PORT  PORTD
#define BUZZER_PIN   PD6

volatile uint16_t buzzer_period = 0;
volatile uint16_t buzzer_cnt = 0;
volatile uint8_t buzzer_enabled = 0;

void buzzer_init(void)
{
	BUZZER_DDR |= (1 << BUZZER_PIN);
	BUZZER_PORT &= ~(1 << BUZZER_PIN);

	// Timer0 normal
	TCCR0A = 0;
	TCCR0B = 0;

	// Prescaler 64 ? ~1 ms por overflow
	TCCR0B |= (1 << CS01) | (1 << CS00);

	TIMSK0 |= (1 << TOIE0);

	sei();
}

void buzzer_update_distance(uint16_t d)
{
	if (d <= 10)
	{
		// som contínuo
		buzzer_enabled = 1;
		BUZZER_PORT |= (1 << BUZZER_PIN);
		buzzer_period = 0;
	}
	else if (d > 100 || d == 0xFFFF)
	{
		// silêncio
		buzzer_enabled = 0;
		BUZZER_PORT &= ~(1 << BUZZER_PIN);
	}
	else
	{
		// mapeia distância ? período
		// 10cm ? rápido | 100cm ? lento
		buzzer_period = (d - 10) * 5; // ajuste fino aqui
		buzzer_enabled = 1;
	}
}

ISR(TIMER0_OVF_vect)
{
	if (!buzzer_enabled || buzzer_period == 0)
	return;

	buzzer_cnt++;

	if (buzzer_cnt >= buzzer_period)
	{
		BUZZER_PORT ^= (1 << BUZZER_PIN);
		buzzer_cnt = 0;
	}
}
