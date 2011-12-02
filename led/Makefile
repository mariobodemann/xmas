#
# Basemakefile used for building and creating a sample hello world programm
#

OPTIONS=-mmcu=atmega32u4 -Wall -Os

all: c asm

asm: led.asm
	avr-as -mmcu=atmega32u4 led.asm -o led-asm.bin

c: led.c
	avr-gcc $(OPTIONS) led.c -c -o led.bin

debug: led.c
	avr-gcc $(OPTIONS) -g led.c -o led-debug.bin

flash: 
	avrdude -p m32u4 -P /dev/ttyACM0 -c avr109 led.bin

flash-asm: 
	avrdude -p m32u4 -P /dev/ttyACM0 -c avr109 led-asm.bin

clean:
	rm *.bin
