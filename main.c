#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "components/hc_sr04/include/hc_sr04.h"
#include "components/buzzer/include/buzzer.h"
#include "components/motor_driver/include/motor.h"

#define RE_PIN PC1

// Calcula o valor de pwm (0-255) a ser enviado para o motor
// de acordo com a distância no modo ré.
uint8_t calc_pwm_limit(uint16_t dist)
{
	if (dist <= 10)
	return 0;

	if (dist >= 100 || dist == 0xFFFF)
	return 255;

	return (uint32_t)(dist - 10) * 255 / 90;
}

int main(void)
{
	// Inicia Motor
	motor_init();
	
	// Inicia HC-SR04
	hcsr04_init();
	
	// Inicia Buzzer
	buzzer_init();

	uint16_t distancia = 0xFFFF;

	while (1)
	{
		// Lê se a ré está ativa
		uint8_t em_re = !(PINC & (1 << RE_PIN));

		if (em_re)
		{
			// Se a ré está ativa monitora a distância
			distancia = hcsr04_get_distance_cm();

			// Envia a distância para o buzzer
			buzzer_update_distance(distancia);

			// Calcula qual deve ser o limite atual do motor e envias para o motor
			uint8_t limit = calc_pwm_limit(distancia);
			motor_set_pwm_limit(limit);
		}
		else
		{
			// Se não estiver em ré, desliga o buzzer e libera o motor
			buzzer_update_distance(0xFFFF);
			motor_set_pwm_limit(255);
		}

		motor_update();

		_delay_ms(60);
	}
}