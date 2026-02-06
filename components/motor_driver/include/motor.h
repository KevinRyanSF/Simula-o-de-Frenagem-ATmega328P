/*
 * motor.h
 *
 * Created: 05/02/2026 21:50:40
 *  Author: kevin
 */ 


#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

// Configura os pinos:
//		Acelerador (PC0 - ADC)
//		Pino de direção (PC1)
//		En da ponte h (PD3 - PWM)
//		In1 e In2 da ponte h (PD4 e PD5)
// Configura o ADC de PC0
// Configura o Timer2 como Fast PWM, usando o canal OC2B
void motor_init(void);

// Configura a direção do motor de acordo com o pino de direção
// Limita a aceleração de acordo com o limite, caso haja
void motor_update(void);

// Para o motor
void motor_brake(void);

// Atualiza o limite
void motor_set_pwm_limit(uint8_t limit);



#endif
