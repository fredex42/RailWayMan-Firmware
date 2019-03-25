MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR -I..

all: test_twi.hex

clean:
	rm -f *.o *.elf *.hex

test_twi.o: test_twi.c ../twi.h ../twi.o
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c test_twi.c

test_twi.elf: test_twi.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o test_twi.elf test_twi.o ../twi.o

test_twi.hex: test_twi.elf
	avr-objcopy -j .text -j .data -O ihex test_twi.elf test_twi.hex
