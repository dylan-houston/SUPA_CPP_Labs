CC=g++
FLAGS=-std=c++20
TARGET=AnalyseData
OBJECTS=CustomFunctions.o AnalyseData.o

${TARGET} : ${OBJECTS}
	${CC} ${FLAGS} ${OBJECTS} -o ${TARGET}
	@make clean

CustomFunctions.o : CustomFunctions.cxx
	${CC} ${FLAGS} -c CustomFunctions.cxx

AnalyseData.o : AnalyseData.cxx
	${CC} ${FLAGS} -c AnalyseData.cxx

clean:
	@rm -f *.o

cleantarget:
	@rm -f ${TARGET}