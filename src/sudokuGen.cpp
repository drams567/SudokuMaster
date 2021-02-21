#include "sudokuGen.h"

using namespace std;

SGen::SGen()
{
	srand(time(NULL));
	init();
}

SGen::SGen(unsigned int seed)
{
	srand(seed);
	init();
}

void SGen::init()
{
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			board[i][k].init(i, k, EMPTY_FLAG);
			emptyList[(i*N) + k] = &board[i][k];
		}
	}
	numEmpty = NUM_SPACES;
}

vector<Space*> SGen::getCousins(Space* inSpace)
{	
	int x = inSpace->index[X];
	int y = inSpace->index[Y];
	vector<Space*> cList;

	// Update region
	int startX;
	if(x == 0)
		startX = x;
	else
		startX = x - (x % REGION_DIM);
	
	int startY;
	if(y == 0)
		startY = y;
	else
		startY = y - (y % REGION_DIM);
	
	int endX = startX + REGION_DIM;
	int endY = startY + REGION_DIM;

	// Fill cousin list
	int z = 0;
	cList.resize(NUM_COUSINS);
	for(int i = startX; i < endX; i++)
	{
		for(int k = startY; k < endY; k++)
		{
			cList.at(z) = &board[i][k];
			z++;
		}
	}

	// Update row and column
	for(int i = 0; i < N; i++)
	{
		cList.at(z) = &board[x][i];
		z++;
		cList.at(z) = &board[i][y];
		z++;
	}
	
	return cList;
}

int SGen::getRandMove(Space* space)
{
	int symbol = rand() % N;
	while(space->vmap[symbol] == false)
	{
		if(symbol == (N -1))
		{
			symbol = 0;
		}
		else
		{
			symbol++;
		}
	}
	
	return symbol;
}

string SGen::genBoard()
{
	int randNumGiven = rand() % (NUM_SPACES - 5) + 1;
	return genBoard(randNumGiven);
}

string SGen::genBoard(const int numGiven)
{
	int randIndex;
	Space* randSpace;
	while(numEmpty > (NUM_SPACES - numGiven))
	{
		randIndex = rand() % numEmpty;
		randSpace = emptyList[randIndex];
		vector<Space*> cList = getCousins(randSpace);
		
	}
	
	return " ";
}

