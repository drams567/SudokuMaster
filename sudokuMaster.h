#ifndef SUDOKU_MASTER_H
#define SUDOKU_MASTER_H

#include <iostream>
#include <string>

using namespace std;

class SMaster;
class Space;

/****** Constants *******/
const int N = 9;
const int NUM_SPACES = N*N;
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
struct Move {
	Space* space;
	int symbol;
	bool guess;
};

class Space {
	friend class SMaster;
	
private:
	int symbol;
	bool fixed;
	int index[2];
	bool vmap[N];	// viability map
	int numv;

public:
	Space(){}
	~Space(){}
	
	void init(int x, int y, int newSymbol)
	{
		symbol = newSymbol;
		index[X] = x;
		index[Y] = y;
		
		if(symbol == EMPTY_FLAG)
		{
			fixed = false;
			for(int i = 0; i < N; i++)
				vmap[i] = true;
			numv = N;
		}
		else
		{
			fixed = true;
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
		cout << " (Totat of " << numv << ")";
	}
};

class SMaster {
private:
	Space board[N][N];
	Space* givenList[NUM_SPACES];
	int numGiven;
	Space* remainList[NUM_SPACES];
	int numRemain;
	
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
			updateCousins(givenList[i]);
		
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
	
	void updateCousins(Space* inSpace)
	{	
		int x = inSpace->index[X];
		int y = inSpace->index[Y];
		int takenSymbol = inSpace->symbol;
	
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
	
		for(int i = startX; i < endX; i++)
		{
			for(int k = startY; k < endY; k++)
			{
				board[i][k].strikeSymbol(takenSymbol);
			}
		}

		// Update row and column
		for(int i = 0; i < N; i++)
		{
			board[x][i].strikeSymbol(takenSymbol);
			board[i][y].strikeSymbol(takenSymbol);
		}
	}
	
	void makeMove(Space* inSpace)
	{
		int symbol = EMPTY_FLAG;
		for(int z = 0; z < N; z++)
		{
			if(inSpace->vmap[z] == true)
			{
				symbol = z;
				break;
			}
		}
		
		inSpace->symbol = symbol;
		updateCousins(inSpace);
	}
	
	void solve()
	{
		while(numRemain > 0)
		{
			bool stuckFlag = true;
			int i = 0;
			while(i < numRemain && remainList[i] != NULL)
			{
				if(remainList[i]->numv == 1)
				{
					makeMove(remainList[i]);
					removeRemain(i);
					stuckFlag = false;
				}
				else
				{
					i++;
				}
			}
			
			if(stuckFlag == true)
			{
				solveAdvanced();
			}
		}
	}
	
	void solveAdvanced()
	{	
		cout << "Cannot be solved trivially..." << endl;
		exit(0);
	}
	
	void solveDebug()
	{
		while(numRemain > 0)
		{
			printBoard();
			dumpSpaces();
			getchar();
		
			bool stuckFlag = true;
			int i = 0;
			while(i < numRemain)
			{
				if(remainList[i]->numv == 1)
				{
					int symbol = EMPTY_FLAG;
					for(int z = 0; z < N; z++)
					{
						if(remainList[i]->vmap[z] == true)
						{
							symbol = z;
							break;
						}
					}
					
					remainList[i]->symbol = symbol;
					updateCousins(remainList[i]);
					removeRemain(i);
					stuckFlag = false;
				}
				else
				{
					i++;
				}
			}
		}
	}
};

#endif