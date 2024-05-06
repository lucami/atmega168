
blink.hex:blink.elf
	avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex
	avr-size --format=avr --mcu=atmega168 blink.elf

blink.elf: blink.o
		avr-gcc -g -mmcu=atmega168 -o blink.elf blink.o
blink.o: blink.c
	avr-gcc -g -Os -Wall -Wshadow -W -mmcu=atmega168 -c blink.c

clean:
	rm blink.elf blink.o blink.hex

load:
	#avrdude -c usbtiny -p m168 -U lfuse:w:0xEF:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m
	avrdude -c usbtiny -p m168 -U lfuse:w:0xEF:m -U hfuse:w:0xdf:m -U efuse:w:0xfc:m
	avrdude -c usbtiny -p m168 -U flash:w:blink.hex

dump:
	avrdude -c usbtiny -p m168 -U flash:r:"./dump.hex":i

erase:
	 avrdude -c usbtiny -p m168  erase

bin:
	avr-objcopy -I ihex -O binary blink.hex blink.bin
