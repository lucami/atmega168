
boot.hex:boot.elf
	avr-objcopy -j .text -j .data -O ihex boot.elf boot.hex
	avr-size --format=avr --mcu=atmega168 boot.elf

boot.elf: boot.o
		avr-gcc -g -mmcu=atmega168 -Wl,-section-start=.text=0x3C00 -o boot.elf boot.o
boot.o: boot.c
	avr-gcc -g -Os -Wall -Wshadow -W -mmcu=atmega168 -c boot.c

clean:
	rm boot.elf boot.o boot.hex

load:
	avrdude -c usbtiny -p m168 -U lfuse:w:0xEF:m -U hfuse:w:0xdf:m -U efuse:w:0xFA:m
	avrdude -c usbtiny -p m168 -U flash:w:boot.hex

dump:
	avrdude -c usbtiny -p m168 -U flash:r:"./dump.hex":i

erase:
	 avrdude -c usbtiny -p m168  erase
