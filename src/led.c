/**
 * Hello World sample with smoothing of colors
 */

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "byte.h"

// include binary image
#include "../assets/earth-transposed.h"

#define DELAY 1
#define LED_BAR_SIZE 30

static void transmit_byte(const Byte b);
static void transmit_some_bits(const Byte b, const unsigned char valid_bit_count);

int main (void)
{
	// setup debug port
	DDRD = 0xFF;
	PORTD= 0xFF;

	// setup led panel for writing (output)
	DDRB = 0xFF;

	// main loop
	unsigned int line = 0;
	while(1)
	{
		// clear port
		PORTB = 0;
		
		int leds_used = 0;
				
		// transmit last byte first, starting with a potentially padded byte
		int last_byte_idx = g_image_width_in_bytes -1 + line*g_image_width_in_bytes;
		if( g_image_width_padding != 0) // padded byte?
		{
			// padding found, so shift byte to first not padded bit
			Byte last = g_image[ last_byte_idx ];
			last.data >>= g_image_width_padding;
			
			// transmit now accurate bits
			transmit_some_bits( last, 8 - g_image_width_padding );
			leds_used += 8 - g_image_width_padding;
			
			// set last index to the last but one (since the last one was just used)
			last_byte_idx --;
		}
		
		// send all remaining bytes
		unsigned start_byte_idx = (line-1)*g_image_width_in_bytes;
		if( line == 0)
		{
			start_byte_idx = 0;
		}
		
		for( unsigned u = start_byte_idx; u < last_byte_idx; ++u)
		{
			transmit_byte( g_image[ u ]);
			leds_used += 8;
		}
		
		// add empty bits if needet
		Byte b;
		b.data = 0;
		transmit_some_bits( b, LED_BAR_SIZE - leds_used);

		// line completed
		PORTB = 1 << 1; 	// show leds
		_delay_ms(DELAY);
 
		line ++;
		line = line % g_image_height;
	}

	// never reached!
   	return 0;
}

static void transmit_some_bits(const Byte b, const unsigned char valid_bit_count)
{
	for( unsigned u = 0; u < valid_bit_count; ++u)
	{
		unsigned char bit = (b.data >> u) % 2;
		PORTB = bit << 0;// set bit
		PORTB = 1 << 2;  // set clock
		PORTB = 0 << 2;  // set clock
	}
}

static void transmit_byte(const Byte b)
{
	transmit_some_bits( b, 8 );
}
