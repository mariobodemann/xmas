/**
 * Hello World sample!
 */

#define F_CPU 16000000
#include <avr/io.h>
//#include <util/delay.h>

int main (void)
{
   DDRB = 0x01; //set up pin 0 on port B
   while (1) //loop forever
   {
       PORTB = 0x01; //set pin 0 on port B high
       PORTB = 0x01; //set pin 0 on port B high
       PORTB = 0x01; //set pin 0 on port B high
       PORTB = 0x01; //set pin 0 on port B high
       PORTB = 0x01; //set pin 0 on port B high
       PORTB = 0x01; //set pin 0 on port B high
       PORTB = 0x01; //set pin 0 on port B high
       PORTB = 0x01; //set pin 0 on port B high
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
       PORTB = 0x00; //set pin 0 on port B low
   }
   return 0;
}
