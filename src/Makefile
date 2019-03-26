.PHONY: common controller

all: common controller

common:
	make -C common

controller: common
	make -C controller

clean:
	make clean -C common
	make clean -C controller
