/*
 * hc_sr04.h
 *
 * Created: 05/02/2026 21:17:41
 *  Author: kevin
 */ 


#ifndef HCSR04_H
#define HCSR04_H

#include <avr/io.h>
#include <avr/interrupt.h>

// Inicia e configura os pinos e o timer utilizado
void hcsr04_init(void);

// Dispara o sinal de trigger
void hcsr04_trigger(void);

// Faz a leitura do pino echo
// Utiliza a interrupção INT0
// Conta quanto tempo o pino ficou em alto utilizando o Timer1
// Envia o valor da distância em cm
uint16_t hcsr04_get_distance_cm(void);

#endif
