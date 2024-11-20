CC=g++
FLAGS=-std=c++20 -w
TARGET=Analyse-Data
OBJECTS=Custom-Functions.o Analyse-Data.o

${TARGET} : ${OBJECTS}
	${CC} ${FLAGS} ${OBJECTS} -o ${TARGET}
	@make clean

Custom-Functions.o : Custom-Functions.cxx Custom-Functions.h
	${CC} ${FLAGS} -c Custom-Functions.cxx

Analyse-Data.o : Analyse-Data.cxx Custom-Functions.h
	${CC} ${FLAGS} -c Analyse-Data.cxx

clean:
	@rm -f *.o

cleantarget:
	@rm -f ${TARGET}