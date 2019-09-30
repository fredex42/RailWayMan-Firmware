MMCU_TYPE = atmega48
CFLAGS = -Os -I../common

all: section_driver.hex

clean:
	rm -f *.o *.elf *.hex *.a

main.o: main.c main.h registers.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

pwm.o: pwm.c pwm.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c pwm.c

trackcircuit.o: trackcircuit.c trackcircuit.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c trackcircuit.c

section_driver.elf: main.o pwm.o trackcircuit.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o section_driver.elf main.o pwm.o trackcircuit.o ../common/common.a

section_driver.hex: section_driver.elf
	avr-objcopy -j .text -j .data -O ihex section_driver.elf section_driver.hex
