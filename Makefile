CC=g++
EX=main
#CFLAGS=

all: main.o rs232.o image/image.o audio/audio.o
	#Create exectuble
	$(CC) -std=c++11 main.o rs232.o image/image.o audio/audio.o `pkg-config --libs opencv` -o $(EX)

main.o: main.cpp
	$(CC) -std=c++11 -c main.cpp

rs232.o: rs232.c rs232.h
	$(CC) -std=c++11 -c rs232.c

image/image.o:
	#Inside image/
	+$(MAKE) -C image

audio/audio.o:
	#Inside audio/
	+$(MAKE) -C audio

clean:
	rm *.o
	rm $(EX)
