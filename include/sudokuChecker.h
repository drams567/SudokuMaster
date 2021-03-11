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
	bool check(string boardString);
	bool check();
	void dumpResults();
};

#endif