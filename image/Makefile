CC=g++
EX=main
#CFLAGS=

all: main.o image.o
	$(CC) -std=c++11 main.o image.o `pkg-config --libs opencv` -o $(EX)

main.o: main.cpp
	$(CC) -std=c++11 -c main.cpp

image.o: image.cpp image.h
	$(CC) -std=c++11 -c `pkg-config --cflags opencv` image.cpp

clean:
	rm -f *.o
	rm -f $(EX)	

#g++ `pkg-config --cflags opencv` facedetect.cpp `pkg-config --libs opencv` -o facedetect
