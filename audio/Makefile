CC=g++
EX=main
#CFLAGS=
#JAVA=jdk-8-oracle-arm32-vfp-hflt
#JAVA=java-8-openjdk-amd64
JAVA=$(JAVA_HOME)
PROC=amd64

all: audio.o
	$(CC) -std=c++11 -I$(JAVA)/include/ -I$(JAVA)/include/linux/ -L$(JAVA)/jre/bin/java -L$(JAVA)/jre/lib/$(PROC)/server/ audio.o -o $(EX) -ljvm

audio.o: audio.cpp voce.h
	$(CC) -std=c++11 -c -g -I$(JAVA)/include/ -I$(JAVA)/include/linux/ -L$(JAVA)/jre/bin/java -L$(JAVA)/jre/lib/$(PROC)/server/ audio.cpp -ljvm
