/*
 * buzzer.h
 *
 * Created: 05/02/2026 22:06:28
 *  Author: kevin
 */ 


#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

// Configura o pino do Buzzer
// Configura o Timer0
void buzzer_init(void);

// Atualiza a frequência de bips do buzzer de acordo com a distância e o timer
// Utiliza a interrupção de Overflow do Timer0
void buzzer_update_distance(uint16_t distance_cm);

#endif
