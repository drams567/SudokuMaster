#ifndef SUDOKU_MASTER_H
#define SUDOKU_MASTER_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class SMaster;
class Space;

/************************/
/****** Constants *******/
const int N = 9;
const int NUM_SPACES = N*N;
const int NUM_COUSINS = N*3;
const int REGION_FACTOR = 3;
const int REGION_DIM = N / REGION_FACTOR;
const int X = 0;
const int Y = 1;
const int EMPTY_FLAG = -1;
const char EMPTY_SYMBOL = '.';

/*********************/
/* Utility Functions */
int convertToSymbol(char charSymbol)
{
	if(charSymbol >= '0')
		return (int)(charSymbol - '0') - 1;
	else
		return EMPTY_FLAG;
}

char convertToPrintSymbol(int inSymbol)
{
	if(inSymbol == EMPTY_FLAG)
		return EMPTY_SYMBOL;
	else
		return ('0' + inSymbol) + 1; // literal acts as an offset
}

/*********************/
/****** Classes ******/
class Space {
	friend class SMaster;
	
private:
	int symbol;		// current symbol, empty space indicated by empty flag constant
	int index[2];	// board position {X, Y}
	bool vmap[N];	// candidate viability map
	int numv;		// num of viable candidates

public:
	// Cosntruct - Destruct //
	Space(){}
	~Space(){}
	
	// Operators //
	void operator=(const Space inSpace)
	{
		symbol = inSpace.symbol;
		index[X] = inSpace.index[X];
		index[Y] = inSpace.index[Y];
		for(int i = 0; i < N; i++)
			vmap[i] = inSpace.vmap[i];
		numv = inSpace.numv;
	}
	
	// Initialize //
	void init(int x, int y, int newSymbol)
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
	
	// Testing //
	void dump()
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
	void strikeSymbol(int badSymbol)
	{
		// Cull candidate symbol
		if(vmap[badSymbol] == true)
		{
			vmap[badSymbol] = false;
			numv--;
		}
	}
};

struct Guess {
	Space* guessSpace;		// Guess Information members
	int guessSymbol;		//
	Space boardState[N][N];				// Game state information members
	Space* remainListState[NUM_SPACES];	//
	int numRemainState;					//
};

class SMaster {
private:
	Space board[N][N];				// Sudoku board
	Space* remainList[NUM_SPACES];	// Remaining spaces to solve
	int numRemain;					// Number of remaining spaces to solve
	Guess guessList[NUM_SPACES];	// Stack of guess moves currently taken
	int numGuess;					// Number of guess moves currently taken
	
public:
	// Constructors //
	SMaster(string boardString)
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
	~SMaster()
	{
		// Empty lists for good practice
		for(int i = 0; i < NUM_SPACES; i++)
		{
			remainList[i] = NULL;
		}
	}
	
	// List Functions //
	void pushRemain(int x, int y)
	{
		remainList[numRemain] = &board[x][y];
		numRemain++;
	}
	
	void pushRemain(Space* inSpace)
	{
		remainList[numRemain] = inSpace;
		numRemain++;
	}
	
	void popRemain()
	{
		numRemain--;
		remainList[numRemain] = NULL;
	}
	
	void removeRemain(int i)
	{
		numRemain--;
		remainList[i] = remainList[numRemain];
		remainList[numRemain] = NULL;
	}
	
	void pushGuess(Guess newGuess)
	{
		guessList[numGuess] = newGuess;
		numGuess++;
	}
	
	Guess popGuess()
	{
		numGuess--;
		return guessList[numGuess];
	}
	
	// Debugging //
	void printBoard()
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
	
	void dumpSpaces()
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
	
	void dumpRemainList()
	{
		cout << "Num remain: " << numRemain << endl;
		for(int i = 0; i < numRemain; i++)
		{
			remainList[i]->dump();
			cout << endl;
		}
	}
	
<<<<<<< HEAD
	// Helpers //	
=======
	void makeMove(Space* space, bool isGuess)
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
		for(int i = 0; i < (int)cList.size(); i++)
		{
			Space* currSpace = cList.at(i);
			currSpace->strikeSymbol(symbol);
		}
	}
	
	void solve()
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
	
>>>>>>> master
	int getBestSpaceIndex()
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
	
	vector<Space*> getCousins(Space* inSpace)
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
	void makeGuess()
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
	
	void makeMove(Space* space, bool isGuess)
	{
		int symbol = EMPTY_FLAG;
		int i = 0;
		for(symbol == EMPTY_FLAG && i < N)
		{
			if(space->vmap[i] == true)
				symbol = z;
			
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
		for(vector<Space*>::iterator = cList.begin(); spaceIt != cList.end(); spaceIt++)
			(*spaceIt)->strikeSymbol(symbol);
		
	}
	
	void restoreFromGuess(Guess badGuess)
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
	void solve()
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
	
};

#endif
