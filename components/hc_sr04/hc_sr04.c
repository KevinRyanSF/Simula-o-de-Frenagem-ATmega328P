/*
 * hc_sr04.c
 *
 * Created: 05/02/2026
 * Author: kevin
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "include/hc_sr04.h"

// TRIG -> PD7
#define TRIG_DDR   DDRD
#define TRIG_PORT  PORTD
#define TRIG_PIN   PD7

// ECHO -> PD2 (INT0)
#define ECHO_DDR   DDRD
#define ECHO_PINR  PIND
#define ECHO_PIN   PD2

volatile uint16_t pulse_width = 0;
volatile uint8_t  measurement_done = 0;
volatile uint8_t  echo_high = 0;

void hcsr04_init(void)
{
    // TRIG saída
    TRIG_DDR |= (1 << TRIG_PIN);
    TRIG_PORT &= ~(1 << TRIG_PIN);

    // ECHO entrada
    ECHO_DDR &= ~(1 << ECHO_PIN);

    // Timer1 normal
    TCCR1A = 0;
    TCCR1B = (1 << CS11); // prescaler 8 ? 0,5 µs

    // INT0 em qualquer mudança
    EICRA |= (1 << ISC00); // any logical change
    EIMSK |= (1 << INT0);

    sei();
}

void hcsr04_trigger(void)
{
    measurement_done = 0;
    echo_high = 0;

    // pulso de trigger
    TRIG_PORT |= (1 << TRIG_PIN);
    _delay_us(10);
    TRIG_PORT &= ~(1 << TRIG_PIN);
}

uint16_t hcsr04_get_distance_cm(void)
{
    hcsr04_trigger();

    uint32_t timeout = 60000;

    while (!measurement_done)
    {
        if (--timeout == 0)
            return 0xFFFF;
    }

    return pulse_width / 116;
}

/* =========================
   ISR INT0 (ECHO)
   ========================= */
ISR(INT0_vect)
{
    if (ECHO_PINR & (1 << ECHO_PIN))
    {
        // subida do ECHO
        TCNT1 = 0;
        echo_high = 1;
    }
    else if (echo_high)
    {
        // descida do ECHO
        pulse_width = TCNT1;
        measurement_done = 1;
        echo_high = 0;
    }
}
