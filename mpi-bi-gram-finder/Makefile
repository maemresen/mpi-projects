TARGET = build
CC = mpicc
RUN = mpirun
EXE = project

ifndef np
override np = 4
endif

ifndef input
override input = input.txt
endif

ifndef output
override output = output.txt
endif

all:	RUN

RUN :	clean build
		@echo == Starting Application with ${np} nodes
		@echo ========================================
		${RUN} -np $(np) $(TARGET)/${EXE} -i $(input) -o $(output)

build:  clean readfile.h filter.h
		@echo == Starting Compiling
		@echo ========================================
		mkdir build
		$(CC) -o $(TARGET)/${EXE} ${EXE}.c
		@echo == Compilation Successful
		@echo ========================================
		
clean:
		rm -rf $(TARGET)