CC=g++
EX=main
STDFLAG=-std=c++11


#USING RECURSIVE
all: main.o dep
	$(CC) $(STDFLAG) main.o INIReader.o ini.o -o $(EX)

main.o: main.cpp
	$(CC) $(STDFLAG) -c -g  main.cpp

dep: INIReader.o ini.o

INIReader.o:	INIReader.cpp INIReader.h
	$(CC) $(STDFLAG) -c -g  INIReader.cpp

ini.o:	ini.c ini.h
	$(CC) $(STDFLAG) -c -g  ini.c

clean:
	rm -f *.o
	rm -f $(EX)
