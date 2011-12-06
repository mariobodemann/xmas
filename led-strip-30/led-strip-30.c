/**
 * Hello World sample with smoothing of colors
 */

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_MIN_DELAY 10
#define DELAY 0

void show_led(float brightness)
{
	PORTB = 0x0;
	_delay_ms( (1.0f-brightness)*LED_MIN_DELAY );
	PORTB = 0xFF;
	_delay_ms( brightness * LED_MIN_DELAY );
}

void show_led_bar( unsigned long bar )
{
	PORTB = 0;
	PORTB = 1 << 1; // update
	
	// loop throgh bar
	for( unsigned int u = 0; u < 30; ++u)
	{
		// shift value to register
		PORTB = 0; // reset clock
		
		// set last bit
		PORTB = bar % 2;
		bar = bar >> 1;
		
		// update clock
		PORTB = 1 << 2;
	}
	
	// update bar
	PORTB = 1 << 1;
}

int main (void)
{
	// setup debug port
	DDRD = 0xFF;
	PORTD = 0xFF;
	_delay_ms( DELAY );
	
	// setup led panel for writing (output)
	DDRB = 0xFF;
	
	// pin 0: sin
	// pin 1: lat
	// pin 2: clk
	// pin 3: rst
	
	// set clock and lat to low
	PORTB = 0x00;
	
	// reset
	
	// shift a one
	PORTB = 1;
	
	// update clock
	PORTB = 1 << 2;
	
	// set latch
	PORTB = 1 << 1;
	PORTB = 0;
	
	// main loop
	unsigned long count = 1;
	while( 1 )
	{		
		show_led_bar(count);
		_delay_ms(DELAY);

		count ++;
	}
	
   	return 0;
}
