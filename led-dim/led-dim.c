/**
 * Hello World sample with smoothing of colors
 */

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_MIN_DELAY 5
#define LED_STEPS 1000

void show_led(float brightness)
{
	PORTB = 0x0;
	_delay_ms( (1.0f-brightness)*LED_MIN_DELAY );
	PORTB = 0xFF;
	_delay_ms( brightness * LED_MIN_DELAY );
}

int main (void)
{
	float increment = 1.0f / LED_STEPS;
	DDRB = 0xFF; //set up all pins on port B
	while (1) //loop forever
	{
		for( float f= 0.0f; f < 1.0f + increment; f+= increment)
		{
			show_led(f);
		}

		for( float f= 0.0f; f < 1.0f + increment; f+= increment)
		{
			show_led(1.0f - f);
		}
   	}
   	return 0;
}
