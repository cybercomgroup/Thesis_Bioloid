CC=g++
EX=main
#CFLAGS=
#JAVA=jdk-8-oracle-arm32-vfp-hflt
#JAVA=java-8-openjdk-amd64
JAVA=$(JAVA_HOME)

all: audio.o main.o
	$(CC) -std=c++11 -I$(JAVA)/include/ -I$(JAVA)/include/linux/ -L$(JAVA)/jre/bin/java -L$(JAVA)/jre/lib/$(PROC)/server/ main.o -o $(EX) -ljvm

main.o: main.cpp
	$(CC) -std=c++11 -c -g -I$(JAVA)/include/ -I$(JAVA)/include/linux/ -L$(JAVA)/jre/bin/java -L$(JAVA)/jre/lib/$(PROC)/server/ main.cpp

audio.o: audio.cpp audio.h
	$(CC) -std=c++11 -c -g -I$(JAVA)/include/ -I$(JAVA)/include/linux/ -L$(JAVA)/jre/bin/java -L$(JAVA)/jre/lib/$(PROC)/server/ audio.cpp -ljvm

clean:
	rm -f *.o
	rm -f $(EX)
