CC=g++
EX=main
SUBDIRS=audio comm image
LINKFILES=main.o comm/rs232.o image/image.o audio/audio.o tools/INIReader.o
STDFLAG=-std=c++11


#MAKE USING RECURSIVE
all: dep $(LINKFILES)
	$(CC) $(STDFLAG) $(LINKFILES) `pkg-config --libs opencv pocketsphinx sphinxbase` -o $(EX)

#This call will go into every SUBDIR and use the makefile inside with 'dep' variable
dep:
	@$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) dep;)

main.o: main.cpp
	$(CC) $(STDFLAG) -c -g  main.cpp

#USEFUL TOOLS
redo: cleanall all

#CLEAN
clean:
	rm -f *.o
	rm -f $(EX)

cleanall: clean
	@$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) clean;)

#OPTIMIZE
optimizeall: optimize optimizedep
	$(CC) $(STDFLAG) -O3 main.o $(LINKFILES) `pkg-config --libs opencv pocketsphinx sphinxbase` -o $(EX)

optimize:
	$(CC) $(STDFLAG) -O3 -c -g  main.cpp

optimizedep:
	@$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) depoptimize;)
