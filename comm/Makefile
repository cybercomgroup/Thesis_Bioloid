CC=g++
EX=main
STDFLAG=-std=c++11

all: main.o comm.o rs232.o
	$(CC) $(STDFLAG) main.o comm.o rs232.o -o $(EX)

main.o: main.cpp
	$(CC) $(STDFLAG) -c main.cpp

comm.o: rs232.o comm.h comm.cpp
	$(CC) $(STDFLAG) -c comm.cpp

rs232.o: rs232.c rs232.h
	$(CC) $(STDFLAG) -c rs232.c

clean:
	rm -f *.o
	rm -f $(EX)

dep: rs232.o
