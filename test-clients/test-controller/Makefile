all: test-controller

test-controller.o: test-controller.c
	gcc -ggdb -c test-controller.c

test-controller: test-controller.o
	gcc -o test-controller test-controller.o
