avrdude -c usbtiny -p m168 -U lfuse:w:0xEF:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m
avrdude -c usbtiny -p m168 -U flash:w:boot.hex
