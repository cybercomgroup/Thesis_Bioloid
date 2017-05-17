CC=g++
EX=main
LINKFILES=main.o image.o ../tools/INIReader.o
STDFLAG=-std=c++11

all: $(LINKFILES)
	$(CC) $(STDFLAG) $(LINKFILES) `pkg-config --libs opencv` -o $(EX)

main.o: main.cpp
	$(CC) $(STDFLAG) -c main.cpp

image.o: image.cpp image.h
	$(CC) $(STDFLAG) -c `pkg-config --cflags opencv` image.cpp

dep: image.o ../tools/INIReader.o

clean:
	rm -f *.o
	rm -f $(EX)

#g++ `pkg-config --cflags opencv` facedetect.cpp `pkg-config --libs opencv` -o facedetect
#Optimize:

optimize:
	$(CC) -std=c++11 -O3 -c main.cpp
	$(CC) -std=c++11 -O3 -c `pkg-config --cflags opencv` image.cpp
	$(CC) -std=c++11 -O3 main.o image.o `pkg-config --libs opencv` -o $(EX)

depoptimize:
	$(CC) $(STDFLAG) -O3 -c image.cpp
