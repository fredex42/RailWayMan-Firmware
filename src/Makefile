.PHONY: common controller

all: common controller section_driver lighttest

common:
	make -C common

lighttest:
	make -C lighttest
	
controller: common
	make -C controller

section_driver: common
	make -C section_driver

clean:
	make clean -C common
	make clean -C controller
	make clean -C section_driver
