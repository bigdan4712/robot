ARM_PREFIX = arm-linux-gnueabi

AR = $(ARM_PREFIX)-ar
CC = $(ARM_PREFIX)-gcc
CPP = $(ARM_PREFIX)-g++
RANLIB = $(ARM_PREFIX)-ranlib

all: binary code39

code39: code39.o ../Support/libev3.a
	$(CPP) -o code39 -L../Support code39.o -lev3

code39.o: code39.cc ../Support/ev3dev.h
	$(CPP) -c -I../Support code39.cc

binary: binary.o ../Support/libev3.a
	$(CPP) -o binary -L../Support binary.o -lev3

binary.o: binary.cc ../Support/ev3dev.h
	$(CPP) -c -I../Support binary.cc

clean:
	rm binary code39 *.o
