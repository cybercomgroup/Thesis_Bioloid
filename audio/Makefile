CC=g++
EX=main
#CFLAGS=
#JAVA=jdk-8-oracle-arm32-vfp-hflt
#JAVA=java-8-openjdk-amd64
JAVA=$(JAVA_HOME)

all: audio.o main.o
	$(CC) -std=c++11 main.o audio.o -o $(EX) `pkg-config --cflags --libs pocketsphinx sphinxbase`

main.o: main.cpp
	$(CC) -std=c++11 -c main.cpp 

audio.o: audio.cpp audio.h
	$(CC) -std=c++11 -c audio.cpp `pkg-config --cflags --libs pocketsphinx sphinxbase`

dep: audio.o

clean:
	rm -f *.o
	rm -f $(EX)
