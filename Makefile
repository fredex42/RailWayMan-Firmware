.PHONY: src

all: src

src:
	make -C src

clean:
	make clean src
