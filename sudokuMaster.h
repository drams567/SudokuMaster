#ifndef SUDOKU_MASTER_H
#define SUDOKU_MASTER_H

#include <iostream>
#include <string>

using namespace std;

class SMaster;

/****** Constants *******/
const int N = 9;
const int NUM_SPACES = N*N;
const int X = 0;
const int Y = 1;
const int EMPTY_FLAG = -1;
const char EMPTY_SYMBOL = '.';
enum State {INITIAL, WORKING, SOLVED, ERROR};

/****** Classes ******/
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
};

class SMaster {
private:
	Space board[N][N];
	int state;
	Space* solvedList[NUM_SPACES];
	int numSolved;
	Space* stagedList[NUM_SPACES];
	int numStaged;
	Space* remainList[NUM_SPACES];
	int numRemain;
	
public:
	SMaster(string boardString)
	{
		// Initialize lists
		for(int i = 0; i < NUM_SPACES; i++)
		{
			solvedList[i] = NULL;
			stagedList[i] = NULL;
			remainList[i] = NULL;
		}
		numSolved = 0;
		numStaged = 0;
		numRemain = 0;
		
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
					pushSolved(i, k);
			
			}
		}
		
		state = INITIAL;
	}
	
	~SMaster()
	{
		// Empty lists for good practice
		for(int i = 0; i < NUM_SPACES; i++)
		{
			solvedList[i] = NULL;
			stagedList[i] = NULL;
			remainList[i] = NULL;
		}
	}
	
	void pushSolved(int x, int y)
	{
		solvedList[numSolved] = &board[x][y];
		numSolved++;
	}
	void pushStaged(int x, int y)
	{
		stagedList[numStaged] = &board[x][y];
		numStaged++;
	}
	void pushRemain(int x, int y)
	{
		remainList[numRemain] = &board[x][y];
		numRemain++;
	}
	
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
	
};

#endif