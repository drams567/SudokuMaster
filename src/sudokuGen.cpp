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

SGen::~SGen()
{
	for(int i = 0; i < NUM_SPACES; i++)
		remainList[i] = NULL;
}

void SGen::init()
{
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			board[i][k].init(i, k, EMPTY_FLAG);
			remainList[(i*N) + k] = &board[i][k];
		}
	}
	numRemain = NUM_SPACES;
}

int SGen::getRandMove(Space* space)
{
	int symbol = rand() % N;
	while(space->vmap[symbol] == false)
	{
		if(symbol == (N - 1))
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
	init();

	int randIndex;
	int randMove;
	Space* randSpace;
	while(numRemain > (NUM_SPACES - numGiven))
	{
		randIndex = rand() % numRemain;
		randSpace = remainList[randIndex];
		vector<Space*> cList = getCousins(randSpace);
		bool goodMove;
		do
		{
			goodMove = true;
			randMove = getRandMove(randSpace);
			for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
			{
				Space* currSpace = *spaceIt;
				if(currSpace->symbol == EMPTY_FLAG && currSpace->numv == 1 && currSpace->vmap[randMove] == true && currSpace != randSpace)
				{
					goodMove = false;
				}
				randSpace->strikeSymbol(randMove);
			}
		} while(goodMove == false);
		
		randSpace->symbol = randMove;
		removeRemain(randIndex);
		for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
		{
			Space* currSpace = *spaceIt;
			currSpace->strikeSymbol(randMove);
		}
	}
	
	return getBoard();
}

void SGen::test(const int numGiven)
{
	init();

	int randIndex;
	int randMove;
	Space* randSpace;
	while(numRemain > (NUM_SPACES - numGiven))
	{
		dumpBoard();
		cout << endl;
		
		randIndex = rand() % numRemain;
		randSpace = remainList[randIndex];
		
		cout << endl << "Space to choose from: ";
		randSpace->dump();
		cout << endl;
		
		vector<Space*> cList = getCousins(randSpace);
		bool goodMove;
		do
		{
			goodMove = true;
			randMove = getRandMove(randSpace);
			
			
			cout << "trying move " << convertToPrintSymbol(randMove) << endl;
			
			for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
			{
				Space* currSpace = *spaceIt;
				if(currSpace->symbol == EMPTY_FLAG && currSpace->numv == 1 && currSpace->vmap[randMove] == true && currSpace != randSpace)
				{
					cout << "bad space: ";
					currSpace->dump();
					cout << endl;
					goodMove = false;
				}
				randSpace->strikeSymbol(randMove);
			}
			
			if(goodMove == false)
			{
				cout << endl << "move " << convertToPrintSymbol(randMove) << " failed" << endl;
				dumpBoard();
				cout << endl;
				dumpRemain();
				cout << endl << "Space to choose from: ";
				randSpace->dump();
				cout << endl;
				getchar();
			}
			
		} while(goodMove == false);
		
		randSpace->symbol = randMove;
		removeRemain(randIndex);
		for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
		{
			Space* currSpace = *spaceIt;
			currSpace->strikeSymbol(randMove);
		}
		
		getchar();
	}
}

