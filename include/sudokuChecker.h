#ifndef SUDOKU_CHECKER_H
#define SUDOKU_CHECKER_H

#include <iostream>
#include <string>
#include "util.h"
#include "defs.h"

using namespace std;

class SChecker 
{
private:
	int board[N][N];
	bool win;
	int numViolate;
	int numMissing;
	
public:
	SChecker();
	SChecker(string boardString);
	~SChecker();

	void init(string boardString);
	int check(string boardString);
	int check();
	void dumpResults();
   
   int getNumViolate();
   int getNumMissing();

};

#endif