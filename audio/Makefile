CC=g++
EX=main
STDFLAG=-std=c++11

all: audio.o main.o
	$(CC) $(STDFLAG) main.o audio.o -o $(EX) `pkg-config --cflags --libs pocketsphinx sphinxbase`

main.o: main.cpp
	$(CC) $(STDFLAG) -c main.cpp

audio.o: audio.cpp audio.h
	$(CC) $(STDFLAG) -c audio.cpp `pkg-config --cflags --libs pocketsphinx sphinxbase`

dep: audio.o

clean:
	rm -f *.o
	rm -f $(EX)
