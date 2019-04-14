MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR -Os -I../common

all: lighttest.hex

clean:
	rm -f *.o *.elf *.hex *.a

main.o: main.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

lighttest.elf: main.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o lighttest.elf main.o

lighttest.hex: lighttest.elf
	avr-objcopy -j .text -j .data -O ihex lighttest.elf lighttest.hex
