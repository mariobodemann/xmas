/**
 * Hello World sample with smoothing of colors
 */

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "/usr/avr/include/math.h"

#define LED_MIN_DELAY 10
#define LED_STEPS 500

void show_led(float brightness)
{
	PORTB = 0x0;
	_delay_ms( (1.0f-brightness)*LED_MIN_DELAY );
	PORTB = 0xFF;
	_delay_ms( brightness * LED_MIN_DELAY );
}

float dim_sin(float f)
{
	return sinf((f-.5)*M_PI)*0.5f+0.5f;
}

float dim_lin(float f)
{
	return f;
}

#define dim dim_sin

int main (void)
{
	float increment = 1.0f / LED_STEPS;
	DDRB = 0xFF; //set up all pins on port B
	
	while (1) //loop forever
	{
		for( float f= increment; f < 1.0f + increment; f+= increment)
		{
			show_led(dim(f));
		}
		_delay_ms(LED_MIN_DELAY);

		for( float f= increment; f < 1.0f + increment; f+= increment)
		{
			show_led(dim(1.0f - f));
		}
		
		// set dark leds
		PORTB = 0x0;
		_delay_ms(LED_MIN_DELAY);
   	}
   	return 0;
}
