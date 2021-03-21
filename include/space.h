#ifndef SUDOKU_SPACE_H
#define SUDOKU_SPACE_H

#include "defs.h"

class Space {
	friend class SMaster;
	friend class SGen;
	
private:
	int symbol;		// current symbol, empty space indicated by empty flag constant
	int index[2];	// board position {X, Y}
	bool vmap[N];	// candidate viability map
	int numv;		// num of viable candidates

public:
	// Cosntruct - Destruct //
	Space();
	~Space();
	
	// Operators //
	void operator=(const Space &inSpace);
	
	// Initialize //
	void init(const int x, const int y, const int newSymbol);
	
	// Testing //
	void dump();

	// Methods //
	bool strikeSymbol(const int badSymbol);
	void getRegion(int &startX, int &startY);
};

#endif