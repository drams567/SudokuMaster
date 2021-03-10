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

	Space saveBoard[N][N];
	int randIndex;
	int randMove;
	Space* randSpace;
	while(numRemain > (NUM_SPACES - numGiven))
	{
		bool goodMove;
		int regX, regY;

		randIndex = rand() % numRemain;
		randSpace = remainList[randIndex];
		randSpace->getRegion(regX, regY);
		vector<Space*> cList = getCousins(randSpace);
		do
		{
			goodMove = true;
			randMove = getRandMove(randSpace);

			// Save board state
			for(int i = 0; i < N; i++)
			{
				for(int k = 0; k < N; k++)
				{
					saveBoard[i][k] = board[i][k];
				}
			}

			// Make move
			randSpace->symbol = randMove;

			// Check if move will move any spaces candidate list to zero
			for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
			{
				Space* currSpace = *spaceIt;
				currSpace->strikeSymbol(randMove);
				if(currSpace->symbol == EMPTY_FLAG && currSpace->numv == 0)
				{
					goodMove = false;
				}
			}
			
			// Check if move will put local region into bad state
			if(goodMove == true)
			{
				int saveSymbol;
				saveSymbol = randSpace->symbol;
				randSpace->symbol = randMove; // simulate move
				if(checkRegionState(regX, regY) == false)
				{
					goodMove = false;
				}
				randSpace->symbol = saveSymbol; // return space to original state
			}

			// Perform culling and check region states
			if(goodMove == true)
			{
				for(int i = 0; i < REGION_DIM; i++)
				{
					for(int k = 0; k < REGION_DIM; k++)
					{	
						int startX = i * REGION_DIM;
						int startY = k * REGION_DIM;
						cullReserved(startX, startY);
					}
				}

				for(int i = 0; i < REGION_DIM; i++)
				{
					for(int k = 0; k < REGION_DIM; k++)
					{
						int startX = i * REGION_DIM;
						int startY = k * REGION_DIM;
						bool state = checkRegionState(startX, startY);
						if(state == false)
						{
							goodMove = false;
						}
					}
				}
			}

			if(goodMove == false)
			{
				// restore board state
				for(int i = 0; i < N; i++)
				{
					for(int k = 0; k < N; k++)
					{
						board[i][k] = saveBoard[i][k];
					}
				}
				randSpace->strikeSymbol(randMove);
				cullReserved(regX, regY);
			}
			
		} while(goodMove == false);
		
		randSpace->symbol = randMove;
		removeRemain(randIndex);
		for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
		{
			Space* currSpace = *spaceIt;
			currSpace->strikeSymbol(randMove);
		}
		
		// Extra culling
		for(int i = 0; i < REGION_FACTOR; i++)
		{
			for(int k = 0; k < REGION_FACTOR; k++)
			{
				int x = i * REGION_DIM;
				int y = k * REGION_DIM;
				cullReserved(x, y);
			}
		}
		
	}
	
	return getBoard();
}

void SGen::test(const int numGiven)
{
	init();

	Space saveBoard[N][N];
	int randIndex;
	int randMove;
	Space* randSpace;
	while(numRemain > (NUM_SPACES - numGiven))
	{
		bool goodMove;
		int regX, regY;

		randIndex = rand() % numRemain;
		randSpace = remainList[randIndex];
		randSpace->getRegion(regX, regY);
		vector<Space*> cList = getCousins(randSpace);
	
		dumpBoard();
		cout << endl;
		cout << endl << "Space to choose from: ";
		randSpace->dump();
		cout << endl;

		do
		{
			goodMove = true;
			randMove = getRandMove(randSpace);

			cout << "trying move " << convertToPrintSymbol(randMove) << endl;

			// Save board state
			for(int i = 0; i < N; i++)
			{
				for(int k = 0; k < N; k++)
				{
					saveBoard[i][k] = board[i][k];
				}
			}

			// Make move
			randSpace->symbol = randMove;

			// Check if move will move any spaces candidate list to zero
			for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
			{
				Space* currSpace = *spaceIt;
				currSpace->strikeSymbol(randMove);
				if(currSpace->symbol == EMPTY_FLAG && currSpace->numv == 0)
				{
					goodMove = false;
				}
			}
			
			// Check if move will put local region into bad state
			if(goodMove == true)
			{
				if(checkRegionState(regX, regY) == false)
				{
					goodMove = false;
					cout << "bad region state avoided" << endl;
				}
			}

			// Perform culling and check region states
			if(goodMove == true)
			{
				for(int i = 0; i < REGION_DIM; i++)
				{
					for(int k = 0; k < REGION_DIM; k++)
					{	
						int startX = i * REGION_DIM;
						int startY = k * REGION_DIM;
						cullReserved(startX, startY);
					}
				}
				for(int i = 0; i < REGION_DIM; i++)
				{
					for(int k = 0; k < REGION_DIM; k++)
					{
						int startX = i * REGION_DIM;
						int startY = k * REGION_DIM;
						bool state = checkRegionState(startX, startY);
						if(state == false)
						{
							goodMove = false;
							cout << "After extra culling, bad region state avoided for " << startX << "," << startY << endl;
						}
					}
				}
			}

			if(goodMove == true)
			{
				for(int i = 0; i < N; i++)
				{
					for(int k = 0; k < N; k++)
					{
						if(board[i][k].symbol == EMPTY_FLAG && board[i][k].numv == 0)
						{
							goodMove = false;
							cout << "Bad space ";
							board[i][k].dump();
							cout << endl;
						}
					}
				}
			}

			if(goodMove == false)
			{
				dumpRemain();
				cout << endl;

				// restore board state
				for(int i = 0; i < N; i++)
				{
					for(int k = 0; k < N; k++)
					{
						board[i][k] = saveBoard[i][k];
					}
				}
				randSpace->strikeSymbol(randMove);
				cullReserved(regX, regY);

				cout << endl << "move " << convertToPrintSymbol(randMove) << " failed" << endl;
				cout << endl;
				getchar();
			}
			dumpRemain();
			cout << endl;
			getchar();
			
		} while(goodMove == false);
		
		removeRemain(randIndex);
		
	}
	
}

