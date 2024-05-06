
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// These are really useful macros that help to get rid of unreadable bit masking code
#define setBit(reg, bit) (reg = reg | (1 << bit))
#define clearBit(reg, bit) (reg = reg & ~(1 << bit))
#define toggleBit(reg, bit) (reg = reg ^ (1 << bit))
#define clearFlag(reg, bit) (reg = reg | (1 << bit))

unsigned char USART_Receive( void ) {
  while ( !(UCSR0A & (1<<RXC0)) )
  ;
  return UDR0;
}

void USART_Flush( void ) {
  unsigned char dummy;
  dummy = UDR0;
}

void send_char(uint8_t c){

  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
  _delay_ms(50); // Wait for 100 ms
  PORTB ^= (1 << PB0); // Toggle the LED
}

void send_str(uint8_t *s){

  for(uint8_t i = 0; i<strlen((char*)s);i++){
    send_char(s[i]);
  }
}

void init_uart_bits(){

  DDRB = 0b00000001;
  PORTB ^= (1 << PB0); // Toggle the LED
  _delay_ms(1000); // Wait for 100 ms

  setBit(UCSR0B, RXEN0);				// Enable reception
  setBit(UCSR0B, TXEN0);				// Enable transmission

  setBit(UCSR0C, UCSZ01);				// 8 bit character size
  setBit(UCSR0C, UCSZ00);
  /*
  clearBit(UCSR0B, RXCIE0);			// We will not enable the receiver interrupt
  clearBit(UCSR0B, TXCIE0);			// We will not enable the transmitter interrupt
  clearBit(UCSR0B, UDRIE0);			// We will not enable the data register empty interrupt
  */


  PORTB ^= (1 << PB0); // Toggle the LED

}

int main(void)
{
  cli();
  _delay_ms(200); // Wait for 100 ms

  uint8_t suca = 'B';



  //init_uart_bits();

  DDRB = 0b00000001;
  PORTB ^= (1 << PB0); // Toggle the LED
  _delay_ms(100); // Wait for 100 ms
  PORTB ^= (1 << PB0); // Toggle the LED


  // Initialize Registers

  // Configure register UCSRA
  //setBit(UCSR0A, U2X0);				// Double the BRG speed (since I am using a 8MHz crystal which is divided by 8)

  clearBit(UCSR0A, MPCM0);			// Normal UART communication

  // Configure register UCSRB
  clearBit(UCSR0B, RXCIE0);			// We will not enable the receiver interrupt
  clearBit(UCSR0B, TXCIE0);			// We will not enable the transmitter interrupt
  clearBit(UCSR0B, UDRIE0);			// We will not enable the data register empty interrupt
  setBit(UCSR0B, RXEN0);				// Enable reception
  setBit(UCSR0B, TXEN0);				// Enable transmission
  clearBit(UCSR0B, UCSZ02);			// 8 bit character size

  //Configure register UCSRC
  clearBit(UCSR0C, UMSEL00);			// Normal Asynchronous Mode
  clearBit(UCSR0C, UMSEL01);
  clearBit(UCSR0C, UPM00);			// No Parity Bits
  clearBit(UCSR0C, UPM01);			// No Parity Bits
  clearBit(UCSR0C, USBS0);			// Use 1 stop bit
  setBit(UCSR0C, UCSZ01);				// 8 bit character size
  setBit(UCSR0C, UCSZ00);
  UBRR0 = MYUBRR;

  _delay_ms(1000); // Wait for 100 ms
  // Configure the baud rate register (this is a combination of both UBRR0L and UBRR0H)
  // Despite using an 8MHz crystal my Fosc is 1MHz since the CLK8DIV fuse bit is dividing the clock
  // by 8. When I try to change this fuse the AVR locks me out!

  USART_Flush();
  USART_Flush();

  uint8_t *str = "Ciaone!\r";

  while (1)
  {
    send_str(str);
    _delay_ms(1000);
    //send_char(suca);
  }
}
