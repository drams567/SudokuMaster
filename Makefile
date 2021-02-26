CC = g++
CCFLAGS = -Wall -O2 -Iinclude -Isrc
EFLAGS = -I/usr/include/eigen3/
CIMGFLAGS = -L/usr/X11R6/lib -lm -lpthread -lX11
LDFLAGS = -lncurses

MAIN = main
EXE = $(MAIN).exe
OBJ = $(MAIN).o sudokuMaster.o sudokuChecker.o sudokuGen.o space.o util.o

vpath %.h include
vpath %.cpp src

$(EXE): $(OBJ)
	$(CC) $(CCFLAGS) -o $(EXE) $(OBJ)

$(MAIN).o: src/$(MAIN).cpp sudokuMaster.o sudokuChecker.o sudokuGen.o space.o include/defs.h
	$(CC) $(CCFLAGS) -c src/$(MAIN).cpp

sudokuChecker.o: src/sudokuChecker.cpp include/sudokuChecker.h util.o include/defs.h
	$(CC) $(CCFLAGS) -c src/sudokuChecker.cpp

sudokuGen.o: src/sudokuGen.cpp include/sudokuGen.h sudokuMaster.o space.o util.o include/defs.h
	$(CC) $(CCFLAGS) -c src/sudokuGen.cpp

sudokuMaster.o: src/sudokuMaster.cpp include/sudokuMaster.h space.o include/defs.h
	$(CC) $(CCFLAGS) -c src/sudokuMaster.cpp

space.o: src/space.cpp include/space.h util.o include/defs.h
	$(CC) $(CCFLAGS) -c src/space.cpp

util.o: util.cpp util.h defs.h
	$(CC) $(CCFLAGS) -c src/util.cpp

BOARD=boards/hard.txt
.PHONY:
run:
	./$(EXE) $(BOARD)
.PHONY:
valrun:
	valgrind ./$(EXE) $(BOARD)
.PHONY:
test:
	./$(EXE) boards/test.txt

.PHONY:
clean:
	find . -type f -name '*.o' -exec rm {} \;
	find . -type f -name '$(EXE)' -exec rm {} \;
	find . -type f -name '*~' -exec rm {} \;
	find . -type f -name '*.swp' -exec rm {} \;
