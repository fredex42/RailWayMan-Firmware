MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR -D DEBUG

.PHONY: test

all: common.a test

clean:
	rm -f *.o *.elf *.hex *.a
	make clean -C test/

test: twi.o
	make -C test/

twi.o: twi.c twi.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c twi.c

address_finder.o: address_finder.c address_finder.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c address_finder.c

common.a: twi.o address_finder.o
	avr-ar rcs common.a twi.o address_finder.o
