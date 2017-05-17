CC=g++
EX=main
LINKFILES=main.o comm.o rs232.o
STDFLAG=-std=c++11

all: $(LINKFILES)
	$(CC) $(STDFLAG) $(LINKFILES) -o $(EX)

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

depoptimize:
	$(CC) $(STDFLAG) -O3 -c rs232.c
