#define F_CPU 16000000UL
#define BAUD 9600
#define LOCALUBRR F_CPU/16/BAUD-1

#define init_gpio  (DDRB = 0b00000010)


#define setBit(reg, bit)    (reg = reg | (1 << bit))
#define clearBit(reg, bit)  (reg = reg & ~(1 << bit))
#define toggleBit(reg, bit) (reg = reg ^ (1 << bit))
#define clearFlag(reg, bit) (reg = reg | (1 << bit))

#include <avr/io.h>
#include <util/delay.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>


uint8_t blinky_test_program_bin[] = {
0x0c, 0x94, 0x34, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x0c, 0x94, 0x3e, 0x00, 0x0c, 0x94, 0x3e, 0x00,
0x11, 0x24, 0x1f, 0xbe, 0xcf, 0xef, 0xd4, 0xe0,
0xde, 0xbf, 0xcd, 0xbf, 0x0e, 0x94, 0x40, 0x00,
0x0c, 0x94, 0x50, 0x00, 0x0c, 0x94, 0x00, 0x00,
0x81, 0xe0, 0x84, 0xb9, 0x91, 0xe0, 0x85, 0xb1,
0x89, 0x27, 0x85, 0xb9, 0x2f, 0xef, 0x31, 0xee,
0x84, 0xe0, 0x21, 0x50, 0x30, 0x40, 0x80, 0x40,
0xe1, 0xf7, 0x00, 0xc0, 0x00, 0x00, 0xf3, 0xcf,
0xf8, 0x94, 0xff, 0xcf
};

uint8_t read_char(){
   while(!(UCSR0A & (1 << RXC0)));
   return UDR0;
}

void send_char(uint8_t c){

  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;

}

void send_str(uint8_t *s){

  for(uint8_t i = 0; i<strlen((char*)s);i++){
      send_char(s[i]);
    }
}

uint8_t USART_receive(void){
    uint16_t timeout=500;

    while( (!(UCSR0A & (1<<RXC0))) && timeout)
    {
      timeout--;
      _delay_ms(10);
    }
    if (timeout==0)
      return 0;
    else
      return UDR0;
}

void write_program(const uint32_t address, const uint8_t *program_buffer, const uint32_t program_buffer_size)
{
  // Disable interrupts.
  uint8_t sreg_last_state = SREG;
  cli();
  eeprom_busy_wait();

  // iterate through the program_buffer one page at a time
  for (uint32_t current_page_address = address;current_page_address < (address + program_buffer_size);current_page_address += SPM_PAGESIZE)
  {
    boot_page_erase(current_page_address);
    boot_spm_busy_wait(); // Wait until the memory is erased.

    // iterate through the page, one word (two bytes) at a time

    for (uint16_t i = 0; i < SPM_PAGESIZE; i += 2)
    {
      uint16_t current_word = 0;
      if ((current_page_address + i) < (address + program_buffer_size))
      {
        // Set up a little-endian word and point to the next word
        current_word = *program_buffer;
        program_buffer++;
        current_word |= (*program_buffer) << 8;
        program_buffer++;
      }
      else
      {
        current_word = 0xFFFF;
      }

      //send_char((uint8_t)0x00FF&current_word);
      //send_char((uint8_t)((0xFF00&current_word)>>8));
      //_delay_ms(100);

      boot_page_fill(current_page_address + i, current_word);
    }

    boot_page_write(current_page_address); // Store buffer in a page of flash memory.
    boot_spm_busy_wait();                  // Wait until the page is written.
  }

  // Re-enable RWW-section. We need this to be able to jump back
  // to the application after bootloading.
  boot_rww_enable();

  // Re-enable interrupts (if they were ever enabled).
  SREG = sreg_last_state;
}

void USART_Init() {
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
  UBRR0 = LOCALUBRR;

}


int main(void)
{
  uint8_t uart_char;
  init_gpio;


  USART_Init();
  uint8_t *str = "Boot\r\n";
  send_str(str);

  PORTB ^= (1 << PB1); // Toggle the LED
  _delay_ms(100); // Wait for 100 ms
  PORTB ^= (1 << PB1); // Toggle the LED


  uart_char = USART_receive();
  _delay_ms(100); // Wait for 100 ms
  send_char(uart_char+1);


  //-----BOOTLOADER-----

  // Check if a user program exists in flash memory
  if (pgm_read_word(0) == 0xFFFF)
  {

    _delay_ms(50);
    write_program(0, blinky_test_program_bin, sizeof(blinky_test_program_bin));
  }


  str = "Writing complete\r";
  send_str(str);
  PORTB ^= (1 << PB1); // Toggle the LED

  _delay_ms(50);
  PORTB ^= (1 << PB1); // Toggle the LED
  _delay_ms(100); // Wait for 100 ms

  // Jump to the start address of the user program (0x0000)
  asm("jmp 0");
}
