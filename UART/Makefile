
uart.hex:uart.elf
	avr-objcopy -j .text -j .data -O ihex uart.elf uart.hex
	avr-size --format=avr --mcu=atmega168 uart.elf

uart.elf: uart.o
		#avr-gcc -g -mmcu=atmega328p -Wl,-section-start=.text=0x1E00 -o uart.elf uart.o
		avr-gcc -g -mmcu=atmega168 -Wl,-section-start=.text=0x0F00 -o uart.elf uart.o
uart.o: uart.c
	avr-gcc -g -Os -Wall -Wshadow -W -mmcu=atmega168 -c uart.c

clean:
	rm uart.elf uart.o uart.hex

load:
	avrdude -c usbtiny -p m168 -U lfuse:w:0xEF:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m
	avrdude -c usbtiny -p m168 -U flash:w:uart.hex

dump:
	avrdude -c usbtiny -p m168 -U flash:r:"./dump.hex":i
