/*
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRB = 0b00000001;

  while (1) {

    PORTB = 0b00000001;
    _delay_ms(1000);
    PORTB = 0b00000000;
    _delay_ms(1000);
  }
}
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRB = 0b00000001;

  while (1)
  {
    PORTB ^= (1 << PB0); // Toggle the LED

    _delay_ms(100); // Wait for 100 ms
  }

  return 0;
}
