CC=g++
EX=main
JAVA=$(JAVA_HOME)
#CFLAGS=

all: main.o rs232.o
	#Create exectuble
	$(CC) -std=c++11 main.o rs232.o -o $(EX)

comm.o: comm.cpp comm.h
	$(CC) -std=c++11 -c comm.cpp

rs232.o: rs232.c rs232.h
	$(CC) -std=c++11 -c rs232.c

dep: rs232.o

clean:
	rm -f *.o
	rm -f $(EX)
