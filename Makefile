CC=g++
EX=main
JAVA=$(JAVA_HOME)
#CFLAGS=


#USING RECURSIVE
all: dep main.o
	#Create exectuble
	$(CC) -std=c++11 main.o comm/rs232.o image/image.o audio/audio.o `pkg-config --libs opencv pocketsphinx sphinxbase` -o $(EX)

redo: cleanall all

#INSIDE THIS FOLDER
allbasic: main.o rs232.o image/image.o
	#Create exectuble
	$(CC) -std=c++11 -L$(JAVA)/jre/lib/$(PROC)/server/ main.o rs232.o image/image.o `pkg-config --libs opencv` -o $(EX) -ljvm

#main.o: main.cpp
	#$(CC) -std=c++11 -c main.cpp

main.o: main.cpp
	$(CC) -std=c++11 -c -g  main.cpp

clean:
	rm -f *.o
	rm -f $(EX)

dep:
	cd image && $(MAKE) dep
	cd audio && $(MAKE) dep
	cd comm && $(MAKE) dep

#INSIDE SUBDIRS
subdirs:
	$(MAKE) -C image
	$(MAKE) -C audio

#Removes all .o files and exectubles inside the project and subfolders
cleanall: clean
	cd image && $(MAKE) clean
	cd audio && $(MAKE) clean
	cd comm && $(MAKE) clean
