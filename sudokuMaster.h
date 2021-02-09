#ifndef SUDOKU_MASTER_H
#define SUDOKU_MASTER_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class SMaster;
class Space;

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

/***** Helpers *****/
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

/****** Classes ******/
class Space {
	friend class SMaster;
	
private:
	int symbol;
	int index[2];
	bool vmap[N];	// viability map
	int numv;

public:
	Space(){}
	~Space(){}
	
	void operator=(const Space inSpace)
	{
		symbol = inSpace.symbol;
		index[X] = inSpace.index[X];
		index[Y] = inSpace.index[Y];
		for(int i = 0; i < N; i++)
			vmap[i] = inSpace.vmap[i];
		numv = inSpace.numv;
	}
	
	void init(int x, int y, int newSymbol)
	{
		symbol = newSymbol;
		index[X] = x;
		index[Y] = y;
		
		if(symbol == EMPTY_FLAG)
		{
			for(int i = 0; i < N; i++)
				vmap[i] = true;
			numv = N;
		}
		else
		{
			for(int i = 0; i < N; i++)
				vmap[i] = false;
			vmap[symbol] = true;
			numv = 1;
		}
	}
	
	void strikeSymbol(int badSymbol)
	{
		if(vmap[badSymbol] == true)
		{
			vmap[badSymbol] = false;
			numv--;
		}
	}
	
	void unstrikeSymbol(int goodSymbol)
	{
		if(vmap[goodSymbol] == false)
		{
			vmap[goodSymbol] = true;
			numv++;
		}
	}
	
	void dump()
	{
		cout << index[X] << "," << index[Y] << ": ";
		for(int i = 0; i < N; i++)
		{
			if(vmap[i] == true)
			{
				cout << convertToPrintSymbol(i) << " ";
			}
		}
		cout << " (Total of " << numv << ")";
	}
};

struct Guess {
	Space* guessSpace;
	int guessSymbol;
	Space boardState[N][N];
	Space* remainListState[NUM_SPACES];
	int numRemainState;
};

class SMaster {
private:
	Space board[N][N];
	Space* givenList[NUM_SPACES];
	int numGiven;
	Space* remainList[NUM_SPACES];
	int numRemain;
	Guess guessList[NUM_SPACES];
	int numGuess;
	
	
public:
	SMaster(string boardString)
	{	
		// Initialize lists
		for(int i = 0; i < NUM_SPACES; i++)
		{
			remainList[i] = NULL;
			givenList[i] = NULL;
		}
		numRemain = 0;
		numGiven = 0;
		numGuess = 0;
		
		// Initialize spaces, updating lists
		for(int i = 0; i < N; i++)
		{
			for(int k = 0; k < N; k++)
			{
				int newSymbol = convertToSymbol(boardString[(i*N) + k]);
				board[i][k].init(i, k, newSymbol);
				
				if(newSymbol == EMPTY_FLAG)
					pushRemain(i, k);
				else
					pushGiven(i, k);
			
			}
		}
		
		for(int i = 0; i < numGiven; i++)
		{
			vector<Space*> cList = getCousins(givenList[i]);
			int symbol = givenList[i]->symbol;
			for(int k = 0; k < (int)cList.size(); k++)
			{
				Space* currSpace = cList.at(k);
				currSpace->strikeSymbol(symbol);
			}
		}
		
	}
	
	~SMaster()
	{
		// Empty lists for good practice
		for(int i = 0; i < NUM_SPACES; i++)
		{
			remainList[i] = NULL;
			givenList[i] = NULL;
		}
	}
	
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
		remainList[numRemain - 1] = NULL;
		numRemain--;
	}
	void removeRemain(int i)
	{
		remainList[i] = remainList[numRemain - 1];
		remainList[numRemain - 1] = NULL;
		numRemain--;
	}
	
	void pushGiven(int x, int y)
	{
		givenList[numGiven] = &board[x][y];
		numGiven++;
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
	
	void makeMove(Space* space, bool isGuess)
	{
		int symbol = EMPTY_FLAG;
		for(int z = 0; z < N; z++)
		{
			if(space->vmap[z] == true)
			{
				symbol = z;
				break;
			}
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
		bool guessFlag = false;
		
		while(numRemain > 0)
		{
			printBoard();
			dumpRemainList();
			getchar();
			
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
				guessFlag = true;
				makeGuess();
			}
		}
	}
	
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
		int takenSymbol = inSpace->symbol;
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
	
};

#endif