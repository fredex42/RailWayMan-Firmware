all: test_controller

test_controller.o: test-controller.c
	gcc -ggdb -c test-controller.c

test_controller: test_controller.o
	gcc -o test_controller test_controller.o
