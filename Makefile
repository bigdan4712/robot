ARM_PREFIX = arm-linux-gnueabi

AR = $(ARM_PREFIX)-ar
CC = $(ARM_PREFIX)-gcc
CPP = $(ARM_PREFIX)-g++
RANLIB = $(ARM_PREFIX)-ranlib

main: main.o ../../Support/libev3.a
	$(CPP) -o test2 -L../../Support test2.o -lev3

main.o: main.cc ../../Support/ev3dev.h
	$(CPP) -c -I../../Support test2.cc

