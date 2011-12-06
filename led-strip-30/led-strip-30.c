/**
 * Hello World sample with smoothing of colors
 */

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// user converted, binary image
#include "../image_to_ascii/image.h"

#define LED_MIN_DELAY 10
#define DELAY 3

void show_led(float brightness)
{
	PORTB = 0x0;
	_delay_ms( (1.0f-brightness)*LED_MIN_DELAY );
	PORTB = 0xFF;
	_delay_ms( brightness * LED_MIN_DELAY );
}

void show_led_bar( long long bar )
	// pin 0: sin
	// pin 1: lat
	// pin 2: clk
	// pin 3: rst	
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

void clear_led_bar()
{
	// set clock and lat to low
	PORTB = 0x00;
	
	// reset
	
	// update clock
	PORTB = 1 << 2;
	
	// set latch
	PORTB = 1 << 1;
	PORTB = 0;
	
}

int main (void)
{
	// setup debug port
	DDRD = 0xFF;
	PORTD = 0xFF;
	_delay_ms( DELAY );
	
	// setup led panel for writing (output)
	DDRB = 0xFF;
	
	// main loop
	long long count = 0;
	while( 1 )
	{	
		clear_led_bar();
		for( unsigned u = 0; u<30; ++u)
		{
			if( count % 2 == 0 )
			{
				show_led_bar( 1ll << u );
			}
			else
			{
				show_led_bar( 1ll << (29-u) );
			}
			_delay_ms(DELAY);
		}
		++count;
	}
	
   	return 0;
}
