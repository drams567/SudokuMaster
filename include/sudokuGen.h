#ifndef SUDOKU_GEN_H
#define SUDOKU_GEN_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include "util.h"
#include "defs.h"
#include "space.h"
#include "sudokuMaster.h"

using namespace std;

class SGen : public SMaster
{
public:
	SGen();
	SGen(const unsigned int seed);
	~SGen();
	
	void init();
	string genBoard();
	string genBoard(const int numGiven);
	int getRandMove(Space* space);
	void makeRandGuess();
	int getRandBestSpaceIndex();
	void makeRandMove();
	
	void test(const int numGiven);
};

#endif