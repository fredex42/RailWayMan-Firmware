.PHONY: common controller

all: common controller section_driver

common:
	make -C common

controller: common
	make -C controller

section_driver: common
	make -C section_driver

clean:
	make clean -C common
	make clean -C controller
	make clean -C section_driver
