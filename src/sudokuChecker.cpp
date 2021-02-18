#include "sudokuChecker.h"

using namespace std;

SChecker::SChecker(string boardString)
{
	numViolate = 0;
	numMissing = 0;
	win = false;
	
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			board[i][k] = convertToSymbol(boardString[(i*N) + k]);
		}
	}
}

SChecker::~SChecker(){}

bool SChecker::check()
{
	int checkList[N];
	int symbol;
	for(int i = 0; i < N; i++)
	{
		// init check list
		for(int z = 0; z < N; z++)
			checkList[z] = 0;
		
		// check rows
		for(int k = 0; k < N; k++)
		{
			symbol = board[i][k];
			if(symbol != EMPTY_FLAG)
			{
				checkList[symbol]++;
				if(checkList[symbol] > 1)
				{
					cout << "Violation: row " << (i+1) << " - placement of extra " << convertToPrintSymbol(symbol) << endl;
					numViolate++;
					win = false;
				}
			}
			else
			{
				numMissing++;
				win = false;
			}
		}
	}
	
	return win;
}

void SChecker::dumpResults()
{
	if(win == true)
		cout << "Game win" << endl;
	if(numViolate > 0)
		cout << "Number of Violations = " << numViolate << endl;
	if(numMissing > 0)
		cout << "Number of empty spaces = " << numMissing << endl;
}