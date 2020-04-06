CXX = cc
OBJS1 = main.o
OBJS2 = main_pr.o
LIBS = -lpthread

all : main main_pr

main : ${OBJS1}
	${CXX} -o $@ ${OBJS1} ${LIBS}

main_pr : ${OBJS2}
	${CXX} -o $@ ${OBJS2} ${LIBS}
clean : 
	rm -vf *.o

