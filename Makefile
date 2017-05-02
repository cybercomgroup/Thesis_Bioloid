CC=g++
EX=main
JAVA=$(JAVA_HOME)
#CFLAGS=


#USING RECURSIVE
all: subdirs main.o rs232.o
	#Create exectuble
	$(CC) -std=c++11 -L$(JAVA)/jre/lib/$(PROC)/server/ main.o rs232.o image/image.o `pkg-config --libs opencv` -o $(EX) -ljvm

cleanandbuild: cleanall all

#INSIDE THIS FOLDER
allbasic: main.o rs232.o image/image.o
	#Create exectuble
	$(CC) -std=c++11 -L$(JAVA)/jre/lib/$(PROC)/server/ main.o rs232.o image/image.o `pkg-config --libs opencv` -o $(EX) -ljvm

#main.o: main.cpp
	#$(CC) -std=c++11 -c main.cpp

main.o: main.cpp
	$(CC) -std=c++11 -c -g -I$(JAVA)/include/ -I$(JAVA)/include/linux/ -L$(JAVA)/jre/bin/java -L$(JAVA)/jre/lib/$(PROC)/server/ main.cpp

rs232.o: rs232.c rs232.h
	$(CC) -std=c++11 -c rs232.c

clean:
	rm -f *.o
	rm -f $(EX)

#INSIDE SUBDIRS
subdirs:
	$(MAKE) -C image
	$(MAKE) -C audio

#Removes all .o files and exectubles inside the project and subfolders
cleanall: clean
	cd image && $(MAKE) clean
	cd audio && $(MAKE) clean
