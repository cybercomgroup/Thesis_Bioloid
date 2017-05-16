CC=g++
EX=main
STDFLAG=-std=c++11

all: main.o image.o
	$(CC) $(STDFLAG) main.o image.o `pkg-config --libs opencv` -o $(EX)

main.o: main.cpp
	$(CC) $(STDFLAG) -c main.cpp

image.o: image.cpp image.h
	$(CC) $(STDFLAG) -c `pkg-config --cflags opencv` image.cpp

dep: image.o

clean:
	rm -f *.o
	rm -f $(EX)

#g++ `pkg-config --cflags opencv` facedetect.cpp `pkg-config --libs opencv` -o facedetect
#Optimize:

optimize:
	$(CC) -std=c++11 -O3 -c main.cpp
	$(CC) -std=c++11 -O3 -c `pkg-config --cflags opencv` image.cpp
	$(CC) -std=c++11 -O3 main.o image.o `pkg-config --libs opencv` -o $(EX)
