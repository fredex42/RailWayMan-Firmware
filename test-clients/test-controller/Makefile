CC=/opt/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc 

all: test-controller

test-controller.o: test-controller.c
	$(CC) -ggdb -c test-controller.c

test-controller: test-controller.o
	$(CC) -o test-controller test-controller.o

clean:
	rm -f test-controller *.o
