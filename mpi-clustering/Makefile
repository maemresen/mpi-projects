CC = mpicc
RUN = mpirun

TARGET = build
SRC=$(wildcard src/*.c)
INC=$(wildcard inc/*.h)

EXE = main

ifndef np
override np = 3
endif

ifndef input
override input = data/news.txt
endif

ifndef output
override output = data/output.txt
endif

ifndef hosts
override hosts = /home/mpiuser/hosts
endif

all:	RUN

RUN :	clean build
		@echo == Starting Application with ${np} nodes
		@echo ========================================
		${RUN} -f ${hosts} -np ${np} ${TARGET}/${EXE} -i ${input} -o ${output}

build:  clean ${INC}
		@echo == Starting Compiling
		@echo ========================================
		mkdir ${TARGET}
		$(CC) -o ${TARGET}/${EXE} ${SRC}
		@echo == Compilation Successful
		@echo ========================================
		
clean:
		rm -rf $(TARGET)
