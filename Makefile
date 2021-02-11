CC = g++
CCFLAGS = -Wall
EFLAGS = -I/usr/include/eigen3/
CIMGFLAGS = -L/usr/X11R6/lib -lm -lpthread -lX11
MAIN = main
OBJ = $(MAIN).o sudokuMaster.o space.o util.o

driver.exe: $(OBJ)
	$(CC) $(CCFLAGS) -o driver.exe $(OBJ)

$(MAIN).o: $(MAIN).cpp sudokuMaster.h space.h defs.h
	$(CC) $(CCFLAGS) -c $(MAIN).cpp

sudokuMaster.o: sudokuMaster.cpp sudokuMaster.h defs.h
	$(CC) $(CCFLAGS) -c sudokuMaster.cpp

space.o: space.cpp space.h defs.h
	$(CC) $(CCFLAGS) -c space.cpp

util.o: util.cpp util.h defs.h
	$(CC) $(CCFLAGS) -c util.cpp

BOARD=boards/hard.txt

.PHONY:
run:
	./driver.exe $(BOARD)

.PHONY:
valrun:
	valgrind ./driver.exe $(BOARD)

.PHONY:
test:
	./driver.exe $(BOARD) test

.PHONY:
clean:

	find . -type f -name '*.o' -exec rm {} \;
	find . -type f -name 'driver.exe' -exec rm {} \;
	find . -type f -name '*~' -exec rm {} \;
	find . -type f -name '*.swp' -exec rm {} \;
