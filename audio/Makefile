CC=g++
EX=main
#CFLAGS=

all: recog.o
	$(CC) -std=c++11 -I/usr/lib/jvm/java-8-openjdk-amd64/include/ -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/ -L/usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java -L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/ recog.o -o $(EX) -ljvm

recog.o: recog.cpp voce.h
	$(CC) -std=c++11 -c -g -I/usr/lib/jvm/java-8-openjdk-amd64/include/ -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/ -L/usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java -L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/ recog.cpp -ljvm

