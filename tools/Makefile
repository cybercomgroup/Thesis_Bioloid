CC=g++
EX=main
JAVA=$(JAVA_HOME)
#CFLAGS=


#USING RECURSIVE
all: main.o INIReader.o ini.o
	$(CC) -std=c++11 main.o INIReader.o ini.o -o $(EX)

main.o: main.cpp
	$(CC) -std=c++11 -c -g  main.cpp

INIReader.o:	INIReader.cpp INIReader.h
	$(CC) -std=c++11 -c -g  INIReader.cpp

ini.o:	ini.c ini.h
	$(CC) -std=c++11 -c -g  ini.c

clean:
	rm -f *.o
	rm -f $(EX)
