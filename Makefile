CC=g++
EX=main
SUBDIRS=audio comm image
STDFLAG=-std=c++11


#MAKE USING RECURSIVE
all: dep main.o
	$(CC) $(STDFLAG) main.o comm/rs232.o image/image.o audio/audio.o `pkg-config --libs opencv pocketsphinx sphinxbase` -o $(EX)

dep:
	@$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir);)

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
