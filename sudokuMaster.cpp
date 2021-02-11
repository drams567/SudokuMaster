#include <iostream>
#include <vector>
#include <string>
#include "sudokuMaster.h"

using namespace std;

// Constructors //
SMaster::SMaster(string boardString)
{	
	// Initialize members
	for(int i = 0; i < NUM_SPACES; i++)
		remainList[i] = NULL;
	numRemain = 0;
	numGuess = 0;

	// Initialize list of starting spaces (spaces which are given to us)
	// This list is used to cull potential candidates of surrounding spaces after the board has been initialized
	Space* givenList[NUM_SPACES];
	int numGiven = 0;
	
	// Initialize board to match board string
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			// Initialize current space with position and value
			int newSymbol = convertToSymbol(boardString[(i*N) + k]);
			board[i][k].init(i, k, newSymbol);
			
			// If space starts empty
			if(newSymbol == EMPTY_FLAG)
			{
				// Add to list of remaining spaces
				pushRemain(i, k);
			}
			else
			{
				// Add to list of starting spaces
				givenList[numGiven] = &board[i][k];
				numGiven++;
			}
		}
	}
	
	// Update candidate lists from starting spaces
	for(int i = 0; i < numGiven; i++)
	{
		// Cull symbol of current starting space from its 'cousin' spaces
		vector<Space*> cList = getCousins(givenList[i]);
		int symbol = givenList[i]->symbol;
		for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
		{
			(*spaceIt)->strikeSymbol(symbol);
		}
	}
	
}

// Destructor //
SMaster::~SMaster()
{
	// Empty lists for good practice
	for(int i = 0; i < NUM_SPACES; i++)
	{
		remainList[i] = NULL;
	}
}

// List Functions //
void SMaster::pushRemain(int x, int y)
{
	remainList[numRemain] = &board[x][y];
	numRemain++;
}

void SMaster::pushRemain(Space* inSpace)
{
	remainList[numRemain] = inSpace;
	numRemain++;
}

void SMaster::popRemain()
{
	numRemain--;
	remainList[numRemain] = NULL;
}

void SMaster::removeRemain(int i)
{
	numRemain--;
	remainList[i] = remainList[numRemain];
	remainList[numRemain] = NULL;
}

void SMaster::pushGuess(Guess newGuess)
{
	guessList[numGuess] = newGuess;
	numGuess++;
}

Guess SMaster::popGuess()
{
	numGuess--;
	return guessList[numGuess];
}

// Debugging //
void SMaster::printBoard()
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

void SMaster::dumpSpaces()
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

void SMaster::dumpRemainList()
{
	cout << "Num remain: " << numRemain << endl;
	for(int i = 0; i < numRemain; i++)
	{
		remainList[i]->dump();
		cout << endl;
	}
}

// Helpers //
int SMaster::getBestSpaceIndex()
{
	int bestSpaceIndex = 0;
	int bestNumV = N + 1; // vnum has a max value of N
	Space* currSpace = NULL;
	for(int i = 0; i < numRemain; i++)
	{
		currSpace = remainList[i];
		if(currSpace->numv < bestNumV)
		{
			bestNumV = currSpace->numv;
			bestSpaceIndex = i;
		}
	}
	
	return bestSpaceIndex;
}

vector<Space*> SMaster::getCousins(Space* inSpace)
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

// Procedures //
void SMaster::makeGuess()
{	
	bool goodGuess = false;
	while(goodGuess == false) 
	{
		goodGuess = true;
		int bestSpaceIndex = getBestSpaceIndex();
		makeMove(remainList[bestSpaceIndex], true);
		vector<Space*> cList = getCousins(remainList[bestSpaceIndex]);
		for(int i = 0; i < (int)cList.size(); i++)
		{
			Space* currSpace = cList.at(i);
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
		}
		else
		{
			removeRemain(bestSpaceIndex);
		}
	}
}

void SMaster::makeMove(Space* space, bool isGuess)
{
	int symbol = EMPTY_FLAG;
	int i = 0;
	while(symbol == EMPTY_FLAG && i < N)
	{
		if(space->vmap[i] == true)
			symbol = i;
		
		i++;
	}
	
	if(isGuess == true)
	{
		Guess guess;
		guess.guessSpace = space;
		guess.guessSymbol = symbol;
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
	}
	
	vector<Space*> cList = getCousins(space);	
	space->symbol = symbol;
	for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
		(*spaceIt)->strikeSymbol(symbol);
	
}

void SMaster::restoreFromGuess(Guess badGuess)
{
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			board[i][k] = badGuess.boardState[i][k];
		}
	}
	for(int i = 0; i < badGuess.numRemainState; i++)
	{
		remainList[i] = badGuess.remainListState[i];
	}
	numRemain = badGuess.numRemainState;
}

// Solve Board //
void SMaster::solve()
{
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
			makeGuess();
		}
	}
}
	