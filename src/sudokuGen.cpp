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
	numGuess = 0;
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

int SGen::getRandBestSpaceIndex()
{
	int bestNumV = N + 1; // vnum has a max value of N
	Space* currSpace = NULL;
	for(int i = 0; i < numRemain; i++)
	{
		currSpace = remainList[i];
		if(currSpace->numv < bestNumV)
		{
			bestNumV = currSpace->numv;
		}
	}

	vector<int> bestSpaces;
	for(int i = 0; i < numRemain; i++)
	{
		if(remainList[i]->numv == bestNumV)
		{
			bestSpaces.push_back(i);
		}
	}
	
	int z = rand() % (int)bestSpaces.size();
	return bestSpaces[z];
}

void SGen::makeRandGuess()
{
	bool goodGuess = false;
	while(goodGuess == false) 
	{
		goodGuess = true;
		int randomBestSpaceIndex = getRandBestSpaceIndex();
		Space* randSpace = remainList[randomBestSpaceIndex];
		int randMove = getRandMove(randSpace);

		// save guess
		Guess guess;
		guess.guessSpace = randSpace;
		guess.guessSymbol = randMove;
		for(int i = 0; i < N; i++)
		{
			for(int k = 0; k < N; k++)
			{
				guess.boardState[i][k] = board[i][k];
			}
		}
		for(int i = 0; i < numRemain; i++)
		{
			guess.remainListState[i] = remainList[i];
			guess.numRemainState = numRemain;
		}
		pushGuess(guess);

		// make move
		randSpace->symbol = randMove;

		// update and check cousins
		vector<Space*> cList = getCousins(remainList[randomBestSpaceIndex]);
		for(int i = 0; i < (int)cList.size(); i++)
		{
			Space* currSpace = cList.at(i);
			currSpace->strikeSymbol(randMove);
			if(currSpace->symbol == EMPTY_FLAG && currSpace->numv == 0)
			{
				goodGuess = false;
			}
		}
		
		if(goodGuess == false)
		{
			Guess badGuess = popGuess();
			restoreFromGuess(badGuess);
			badGuess.guessSpace->strikeSymbol(badGuess.guessSymbol);
			statNumBadGuess++;
		}
		else
		{
			removeRemain(randomBestSpaceIndex);
		}
	}
}

string SGen::genBoard(const int numGiven)
{
	init();

	bool stuckFlag;
	while(numRemain > 0)
	{
		stuckFlag = true;
		int i = 0;
		while(i < numRemain)
		{
			if(remainList[i]->numv == 1)
			{
				makeMove(remainList[i], false);
				statNumMoves++;
				removeRemain(i);
				stuckFlag = false;
			}
			else if(remainList[i]->numv == 0)
			{
				Space* badSpace = remainList[i];
				while(badSpace->numv == 0)
				{
					Guess badGuess = popGuess();
					restoreFromGuess(badGuess);
					statNumBadGuess++;
					badGuess.guessSpace->strikeSymbol(badGuess.guessSymbol);
				}
			}
			else
			{
				i++;
			}
		}
		
		if(stuckFlag == true)
		{
			makeRandGuess();
		}
	}
	
	return getBoard();
}

void SGen::test(const int numGiven)
{
	init();

	bool stuckFlag;
	while(numRemain > 0)
	{
		stuckFlag = true;
		int i = 0;
		while(i < numRemain)
		{
			if(remainList[i]->numv == 1)
			{
				makeMove(remainList[i], false);
				statNumMoves++;
				removeRemain(i);
				stuckFlag = false;
			}
			else if(remainList[i]->numv == 0)
			{
				Space* badSpace = remainList[i];
				while(badSpace->numv == 0)
				{
					Guess badGuess = popGuess();
					restoreFromGuess(badGuess);
					statNumBadGuess++;
					badGuess.guessSpace->strikeSymbol(badGuess.guessSymbol);
				}
			}
			else
			{
				i++;
			}
		}
		
		if(stuckFlag == true)
		{
			makeRandGuess();
		}
	}
	
	dumpBoard();
	cout << endl;

}

