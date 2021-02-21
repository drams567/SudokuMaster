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

using namespace std;

class SGen 
{
private:
	Space board[N][N];
	Space* emptyList[NUM_SPACES];
	int numEmpty;

public:
	SGen();
	SGen(unsigned int seed);
	~SGen();
	
	void init();
	string genBoard();
	string genBoard(const int numGiven);
	vector<Space*> getCousins(Space* inSpace);
	int getRandMove(Space* space);
};

#endif