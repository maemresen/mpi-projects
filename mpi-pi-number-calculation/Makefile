TARGET = build
CC = mpicc

ifndef np
override np = 4
endif

all:	run

build:  clean
	mkdir build
	$(CC) -o $(TARGET)/PiNumberCalculator PiNumberCalculator.c

run :	clean build
	mpirun -np $(np) $(TARGET)/PiNumberCalculator

clean:
	rm -rf $(TARGET)