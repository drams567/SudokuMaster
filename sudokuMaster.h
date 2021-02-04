#ifndef SUDOKU_MASTER_H
#define SUDOKU_MASTER_H

#include <iostream>
#include <string>

using namespace std;

/****** Constants *******/
const int N = 9;
const int NUM_SPACES = N*N;
const int X = 0;
const int Y = 1;
const int EMPTY_FLAG = -1;
const char EMPTY_SYMBOL = '.';

class SMaster;

class Space {
	friend class SMaster;
	
private:
	int symbol;
	bool fixed;
	int index[2];

public:
	Space(){}
	~Space(){}
	
	void init(int x, int y, int newSymbol)
	{
		symbol = newSymbol;
		index[X] = x;
		index[Y] = y;
		
		if(symbol == EMPTY_FLAG)
			fixed = false;
		else
			fixed = true;
	}

	void operator=(const Space &inSpace)
	{
		symbol = inSpace.symbol;
		fixed = inSpace.fixed;
		index[X] = inSpace.index[X];
		index[Y] = inSpace.index[Y];
	}
};

class SMaster {
private:
	Space board[N][N];
	
public:
	SMaster(string boardString)
	{
		for(int i = 0; i < N; i++)
		{
			for(int k = 0; k < N; k++)
			{
				int newSymbol = convertToSymbol(boardString[(i*N) + k]);
				board[i][k].init(i, k, newSymbol);
			}
		}
	}
	
	~SMaster(){}
	
	int convertToSymbol(char charSymbol)
	{
		if(charSymbol >= '0')
		{
			return (int)(charSymbol - '0');
		}
		else
		{
			return EMPTY_FLAG;
		}
	}
	
	char convertToPrintSymbol(int inSymbol)
	{
		if(inSymbol == EMPTY_FLAG)
		{
			return EMPTY_SYMBOL;
		}
		
		return '0' + inSymbol; // literal acts as an offset
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
				{
					cout << "|";
				}
				cout << " " << convertToPrintSymbol(board[i][k].symbol) << " ";
			}
			cout << " |" << endl;
			if(i % 3 == 2)
			{
				cout << string(N*3 + 5, '-') << endl;
			}
			else
			{
				cout << endl;
			}
		}
		
		cout << endl;
	}
};

#endif