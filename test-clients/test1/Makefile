CC=/opt/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

all: test1

test1: test1.o
	$(CC) -o test1 test1.o

test1.o: test1.c
	$(CC) -c test1.c

clean:
	rm -f *.o test1
