CC=g++
EX=main
SUBDIRS=audio comm image
LINKFILES=comm/rs232.o image/image.o audio/audio.o
STDFLAG=-std=c++11


#MAKE USING RECURSIVE
all: dep main.o
	$(CC) $(STDFLAG) main.o $(LINKFILES) `pkg-config --libs opencv pocketsphinx sphinxbase` -o $(EX)

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
optimize: depoptimize
	$(CC) $(STDFLAG) -O3 -c -g  main.cpp
	$(CC) $(STDFLAG) -O3 main.o $(LINKFILES) `pkg-config --libs opencv pocketsphinx sphinxbase` -o $(EX)

depoptimize:
	@$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) depoptimize;)
