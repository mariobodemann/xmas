/**
 * Hello World sample!
 */

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
 
int main (void)
{
   DDRB = 0xFF; //set up all pins on port B
   while (1) //loop forever
   {
       PORTB = 0xFF; //set pin 0 on port B high
       _delay_ms(500);
       PORTB = 0x00; //set pin 0 on port B low
       _delay_ms(500);
   }
   return 0;
}
