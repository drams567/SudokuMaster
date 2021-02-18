#include <iostream>

#include "space.h"
#include "util.h"
#include "defs.h"

using namespace std;

// Cosntruct - Destruct //
Space::Space(){}
Space::~Space(){}

// Operators //
void Space::operator=(Space inSpace)
{
	symbol = inSpace.symbol;
	index[X] = inSpace.index[X];
	index[Y] = inSpace.index[Y];
	for(int i = 0; i < N; i++)
		vmap[i] = inSpace.vmap[i];
	numv = inSpace.numv;
}

// Initialize //
void Space::init(int x, int y, int newSymbol)
{
	symbol = newSymbol;
	index[X] = x;
	index[Y] = y;
	
	// Space starts empty
	if(symbol == EMPTY_FLAG)
	{
		for(int i = 0; i < N; i++)
			vmap[i] = true;
		numv = N;
	}
	// Symbol is given
	else
	{
		for(int i = 0; i < N; i++)
			vmap[i] = false;
		vmap[symbol] = true;
		numv = 1;
	}
}

// Methods //
void Space::strikeSymbol(int badSymbol)
{
	// Cull candidate symbol
	if(vmap[badSymbol] == true)
	{
		vmap[badSymbol] = false;
		numv--;
	}
}

// Testing //
void Space::dump()
{
	// Print index
	cout << index[X] << "," << index[Y] << ": ";
	
	// Print list of viable candidates
	for(int i = 0; i < N; i++)
	{
		if(vmap[i] == true)
		{
			cout << convertToPrintSymbol(i) << " ";
		}
	}
	
	// Print number of viable candidates
	cout << " (Total of " << numv << ")";
}
