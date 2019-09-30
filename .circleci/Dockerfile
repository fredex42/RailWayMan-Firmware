FROM ubuntu:18.10

RUN apt-get -y update && apt-get -y upgrade && apt-get -y install avr-libc binutils-avr gcc-avr gdb-avr avrdude make python-pip git && apt-get clean
RUN pip install awscli
RUN useradd cibuild
RUN mkdir -p /home/cibuild; chown cibuild /home/cibuild
USER cibuild
