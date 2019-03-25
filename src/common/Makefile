MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR -D DEBUG

.PHONY: test

all: twi.o test

clean:
	rm -f *.o *.elf *.hex
	make clean -C test/

test: twi.o
	make -C test/

twi.o: twi.c twi.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c twi.c
