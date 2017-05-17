CC=g++
EX=main
STDFLAG=-std=c++11

all: audio.o main.o
	$(CC) $(STDFLAG) main.o audio.o -o $(EX) `pkg-config --libs pocketsphinx sphinxbase`

main.o: main.cpp
	$(CC) $(STDFLAG) -c main.cpp

audio.o: audio.cpp audio.h
	$(CC) $(STDFLAG) -c audio.cpp `pkg-config --cflags pocketsphinx sphinxbase`

dep: audio.o

clean:
	rm -f *.o
	rm -f $(EX)

depoptimize:
	$(CC) $(STDFLAG) -O3 -c audio.cpp `pkg-config --cflags pocketsphinx sphinxbase`
