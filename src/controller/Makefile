MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR -Os -I../common

all: controller.hex

clean:
	rm -f *.o *.elf *.hex *.a

main.o: main.c main.h registers.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

timeout.o: timeout.c timeout.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c timeout.c

adc.o: adc.c  adc.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c adc.c

led_output.o: led_output.c led_output.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c led_output.c

controller.elf: main.o timeout.o adc.o led_output.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o controller.elf main.o timeout.o led_output.o adc.o ../common/common.a

controller.hex: controller.elf
	avr-objcopy -j .text -j .data -O ihex controller.elf controller.hex
