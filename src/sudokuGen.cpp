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
		emptyList[i] = NULL;
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
	vector<Space*> cList;
	int startX, startY, endX, endY;
	int x, y;
	
	// Get region
	inSpace->getRegion(startX, startY);
	endX = startX + REGION_DIM;
	endY = startY + REGION_DIM;

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

	// Get row and column
	x = inSpace->index[X];
	y = inSpace->index[Y];

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

void SGen::removeEmpty(int i)
{
	numEmpty--;
	emptyList[i] = emptyList[numEmpty];
	emptyList[numEmpty] = NULL;
}

string SGen::getBoard()
{
	string boardString(NUM_SPACES, ' ');
	int z = 0;
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			z = (i * N) + k;
			boardString[z] = convertToPrintSymbol(board[i][k].symbol);
		}
	}
	
	return boardString;
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
	while(numEmpty > (NUM_SPACES - numGiven))
	{
		randIndex = rand() % numEmpty;
		randSpace = emptyList[randIndex];
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
		removeEmpty(randIndex);
		for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
		{
			Space* currSpace = *spaceIt;
			currSpace->strikeSymbol(randMove);
		}
	}
	
	return getBoard();
}

// Debugging //
void SGen::dumpBoard()
{
	cout << endl;
	cout << string(N*3 + 5, '-') << endl;
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			if(k % 3 == 0)
				cout << "|";
			
			cout << " " << convertToPrintSymbol(board[i][k].symbol) << " ";
		}
		cout << " |" << endl;
		if(i % 3 == 2)
			cout << string(N*3 + 5, '-') << endl;
		else
			cout << endl;
	}
	
	cout << endl;
}

void SGen::dumpSpaces()
{
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			board[i][k].dump();
			cout << endl;
		}
	}
}

void SGen::dumpEmpty()
{
	for(int i = 0; i < numEmpty; i++)
	{
		emptyList[i]->dump();
		cout << endl;
	}
}

void SGen::test(const int numGiven)
{
	init();

	int randIndex;
	int randMove;
	Space* randSpace;
	while(numEmpty > (NUM_SPACES - numGiven))
	{
		dumpBoard();
		cout << endl;
		
		randIndex = rand() % numEmpty;
		randSpace = emptyList[randIndex];
		
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
				dumpEmpty();
				cout << endl << "Space to choose from: ";
				randSpace->dump();
				cout << endl;
				getchar();
			}
			
		} while(goodMove == false);
		
		randSpace->symbol = randMove;
		removeEmpty(randIndex);
		for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
		{
			Space* currSpace = *spaceIt;
			currSpace->strikeSymbol(randMove);
		}
		
		getchar();
	}
}

