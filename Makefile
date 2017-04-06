CC=g++
EX=main
#CFLAGS=

all: main.o rs232.o
	$(CC) -std=c++11 main.o rs232.o -o $(EX)

main.o: main.cpp
	$(CC) -std=c++11 -c main.cpp

rs232.o: rs232.c rs232.h
	$(CC) -std=c++11 -c rs232.c