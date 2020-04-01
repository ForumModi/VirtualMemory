CXX = cc
OBJS1 = main.o
LIBS = -lpthread

all : main

main : ${OBJS1}
	${CXX} -o $@ ${OBJS1} ${LIBS}

clean : 
	rm -vf *.o

