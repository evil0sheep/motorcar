MOTORCAR_DIR=../../../..
CC=g++


INCLUDES= -I${MOTORCAR_DIR}/src/compositor -I${MOTORCAR_DIR}/src/protocol -I${MOTORCAR_DIR}/src/device
LIBS= -L${MOTORCAR_DIR}/lib -lmotorcar-compositor 
CFLAGS=-std=c++11 
objects=oculushmd.o sixensemotionsensingsystem.o sixensecontrollernode.o sixensebasenode.o 

compositor: main.cpp
	${CC}  main.cpp  ${CFLAGS} ${INCLUDES} ${LIBS} -o simple-compositor 

clean:
	rm simple-compositor 