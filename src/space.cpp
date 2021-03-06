#include <iostream>

#include "space.h"
#include "util.h"
#include "defs.h"

using namespace std;

// Cosntruct - Destruct //
Space::Space(){}
Space::~Space(){}

// Operators //
void Space::operator=(const Space &inSpace)
{
	symbol = inSpace.symbol;
	index[X] = inSpace.index[X];
	index[Y] = inSpace.index[Y];
	for(int i = 0; i < N; i++)
		vmap[i] = inSpace.vmap[i];
	numv = inSpace.numv;
}

// Helpers //
void Space::init(const int x, const int y, const int newSymbol)
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
	// Space starts as given
	else
	{
		for(int i = 0; i < N; i++)
			vmap[i] = false;
		vmap[symbol] = true;
		numv = 1;
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

// Methods //
bool Space::strikeSymbol(const int badSymbol)
{
	// Cull candidate symbol
	if(vmap[badSymbol] == true)
	{
		vmap[badSymbol] = false;
		numv--;
		return true;
	}
	return false;
}

void Space::getRegion(int &startX, int &startY)
{
	int x = index[X];
	int y = index[Y];

	// Get region starting row
	if(x == 0)
		startX = x;
	else
		startX = x - (x % REGION_DIM);

	// Get region starting column
	if(y == 0)
		startY = y;
	else
		startY = y - (y % REGION_DIM);
}