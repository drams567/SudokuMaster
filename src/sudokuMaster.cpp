#include <iostream>
#include <vector>
#include <string>
#include "sudokuMaster.h"

using namespace std;

// Constructors //
SMaster::SMaster() 
{
	string boardString(NUM_SPACES, EMPTY_FLAG);
	init(boardString);
}

SMaster::SMaster(const string boardString)
{
	init(boardString);
}

// Destructor //
SMaster::~SMaster()
{
	// Empty lists for good practice
	for(int i = 0; i < NUM_SPACES; i++)
		remainList[i] = NULL;

}

// List Functions //
void SMaster::pushRemain(const int x, const int y)
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

void SMaster::removeRemain(const int i)
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

// Setters and Getters //
string SMaster::getBoard()
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

int SMaster::getNumGuess() { return statNumGuess; }
int SMaster::getNumBadGuess() { return statNumBadGuess; }
int SMaster::getNumMoves() { return statNumMoves; }

// Debugging //
void SMaster::dumpBoard()
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

void SMaster::dumpRemain()
{
	cout << "Num remain: " << numRemain << endl;
	for(int i = 0; i < numRemain; i++)
	{
		remainList[i]->dump();
		cout << endl;
	}
}

void SMaster::dumpStats()
{
	cout << statNumGuess << " guesses (" << (statNumGuess-statNumBadGuess) << " good, " << statNumBadGuess << " bad)" << endl;
	cout << statNumMoves << " moves made" << endl;
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

bool SMaster::checkRegionState(const int startX, const int startY)
{
	bool regRemainList[N];
	bool regVList[N];
	
	int endX = startX + REGION_DIM;
	int endY = startY + REGION_DIM;

	for(int i = 0; i < N; i++)
	{
		regRemainList[i] = true;	// Assume all remain
		regVList[i] = false;		// Assume no spaces have candidates
	}

	// Mark symbols missing from region, and symbols that can be placed in region
	for(int x = startX; x < endX; x++)
	{
		for(int y = startY; y < endY; y++)
		{
			int symbol = board[x][y].symbol;
			if(symbol != EMPTY_FLAG)
			{
				regRemainList[symbol] = false;
			}
			else
			{
				for(int i = 0; i < N; i++)
				{
					if(board[x][y].vmap[i] == true)
					{
						regVList[i] = true;
					}
				}
			}
		}
	}

	// Search for symbol that is missing and cannot be placed
	for(int i = 0; i < N; i++)
	{
		if(regRemainList[i] == true && regVList[i] == false)
		{
			return false;
		}
	}

	return true;
}

// Procedures //
void SMaster::init(const string boardString)
{
   // Initialize stats
	statNumGuess = 0;
	statNumBadGuess = 0;
	statNumMoves = 0;

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


void SMaster::makeGuess()
{	
	bool goodGuess = false;
	while(goodGuess == false) 
	{
		goodGuess = true;
		int bestSpaceIndex = getBestSpaceIndex();
		Space* bestSpace = remainList[bestSpaceIndex];

		if (bestSpace->numv == 0)
		{
			while (bestSpace->numv == 0)
			{
				recallLastGuess();
			}
			goodGuess = false;
		}
		else
		{
			makeMove(bestSpace, true);
			vector<Space*> cList = getCousins(bestSpace);
			for (int i = 0; i < (int)cList.size(); i++)
			{
				Space* currSpace = cList.at(i);
				if (currSpace->symbol == EMPTY_FLAG && currSpace->numv == 0)
				{
					goodGuess = false;
				}
			}
			
			if(goodGuess == true)
			{
				for(int i = 0; i < REGION_DIM; i++)
				{
					for(int k = 0; k < REGION_DIM; k++)
					{
						int regX, regY;
						regX = i * REGION_DIM;
						regY = k * REGION_DIM;
						if(checkRegionState(regX, regY) == false)
						{
							goodGuess = false;
						}
					}
				}
			}

			if (goodGuess == false)
			{
				recallLastGuess();
			}
			else
			{
				removeRemain(bestSpaceIndex);
			}
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
		saveStateAsGuess(space, symbol);
		statNumGuess++;
	}
	
	space->symbol = symbol;
	vector<Space*> cList = getCousins(space);
	for(vector<Space*>::iterator spaceIt = cList.begin(); spaceIt != cList.end(); spaceIt++)
		(*spaceIt)->strikeSymbol(symbol);
	
	statNumMoves++;
}

void SMaster::recallLastGuess()
{
	Guess badGuess = popGuess();
	
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
	
	badGuess.guessSpace->strikeSymbol(badGuess.guessSymbol);

	statNumBadGuess++;
}

void SMaster::cullReserved(const int startX, const int startY)
{
	// Init
	int endX = startX + REGION_DIM;
	int endY = startY + REGION_DIM;
	vector<int> rowList[N];
	vector<int> colList[N];

	// Get list of candidate spaces for missing symbols
	for(int x = startX; x < endX; x++)
	{
		for(int y = startY; y < endY; y++)
		{
			if(board[x][y].symbol == EMPTY_FLAG)
			{
				for(int i = 0; i < N; i++)
				{
					if(board[x][y].vmap[i] == true)
					{
						rowList[i].push_back(x);
						colList[i].push_back(y);
					}
				}
			}
		}
	}
	
	// Check for distinct rows or columns and perform culling
	for(int i = 0; i < N; i++)
	{
		// init
		int numRows = (int)rowList[i].size();
		int numCols = (int)colList[i].size();
		bool distinctRow;
		bool distinctCol;
		int row = -1;
		int col = -1;
		int k;
		
		// check for distinct row
		if(numRows > 0)
		{
			distinctRow = true;
			row = rowList[i].at(0);
			k = 0;
			while(k < numRows && distinctRow == true)
			{
				if(rowList[i].at(k) != row)
				{
					distinctRow = false;
				}
				k++;
			}
		}
		else
		{
			distinctRow = false;
		}
		
		// check for distinct column
		if(numCols > 0)
		{
			distinctCol = true;
			col = colList[i].at(0);
			k = 0;
			while(k < numCols && distinctCol == true)
			{
				if(colList[i].at(k) != col)
				{
					distinctCol = false;
				}
				k++;
			}
		}
		else
		{
			distinctCol = false;
		}
		
		// Perform culling
		vector<Space*> callList;
		if(distinctRow)
		{
			for(int z = 0; z < startY; z++)
			{
				if(board[row][z].strikeSymbol(i))
				{
					callList.push_back(&board[row][z]);
				}
			}
			for(int z = endY; z < N; z++)
			{
				if(board[row][z].strikeSymbol(i))
				{
					callList.push_back(&board[row][z]);
				}
			}
		}
		if(distinctCol)
		{
			for(int z = 0; z < startX; z++)
			{
				if(board[z][col].strikeSymbol(i))
				{
					callList.push_back(&board[z][col]);
				}
			}
			for(int z = endX; z < N; z++)
			{
				if(board[z][col].strikeSymbol(i))
				{
					callList.push_back(&board[z][col]);
				}
			}
		}
		
		int regX, regY;
		for(int z = 0; z < (int)callList.size(); z++)
		{
			callList.at(z)->getRegion(regX, regY);
			cullReserved(regX, regY);
		}
	}	
}

void SMaster::saveStateAsGuess(Space* space, int move)
{
	Guess guess;
	guess.guessSpace = space;
	guess.guessSymbol = move;
	for (int i = 0; i < N; i++)
	{
		for (int k = 0; k < N; k++)
		{
			guess.boardState[i][k] = board[i][k];
		}
	}

	for (int i = 0; i < numRemain; i++)
	{
		guess.remainListState[i] = remainList[i];
		guess.numRemainState = numRemain;
	}

	pushGuess(guess);
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
					recallLastGuess();
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

void SMaster::solve(const string boardString)
{
   init(boardString);
   solve();
}

void SMaster::testSolve()
{
	int z = NUM_SPACES;
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
					recallLastGuess();
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
		
		if(z > numRemain)
		{
			z = numRemain;
			cout << "Lowest remaining so far = " << z << endl;
		}
	}
}