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
	SChecker(const string boardString);
	~SChecker();

	void init(const string boardString);
	int check(const string boardString);
	int check();
	void dumpResults();
   
   int getNumViolate();
   int getNumMissing();

};

#endif