#include "sudokuChecker.h"

using namespace std;

SChecker::SChecker()
{
	string boardString(NUM_SPACES,EMPTY_FLAG);
	init(boardString);
}

SChecker::SChecker(string boardString)
{
	init(boardString);
}

SChecker::~SChecker(){}

void SChecker::init(string boardString)
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

bool SChecker::check()
{
	win = true;
	
	// Check regional squares and number of missing spaces
	int regCheckList[N];
	int regSymbol;
	for(int i = 0; i < REGION_DIM; i++)
	{
		for(int k = 0; k < REGION_DIM; k++)
		{
			for(int z = 0; z < N; z++)
				regCheckList[z] = 0;
			
			int startX = i * REGION_DIM;
			int startY = k * REGION_DIM;
			int endX = startX + REGION_DIM;
			int endY = startY + REGION_DIM;
			for(int x = startX; x < endX; x++)
			{
				for(int y = startY; y < endY; y++)
				{
					regSymbol = board[x][y];
					if(regSymbol != EMPTY_FLAG)
					{
						regCheckList[regSymbol]++;
						if(regCheckList[regSymbol] > 1)
						{
							cout << "Violation: region " << ((i * REGION_DIM) + k + 1) << " - placement of extra " << convertToPrintSymbol(regSymbol) << endl;
							numViolate++;
							win = false;
						}
					}
					else // Only place we note a missing space
					{
						win = false;
						numMissing++; // Only place we count number of missing spaces
					}
				}
			}
		}
	}
	
	// Check rows and cols
	int rowCheckList[N];
	int colCheckList[N];
	int rowSymbol;
	int colSymbol;
	for(int i = 0; i < N; i++)
	{
		// init check list
		for(int z = 0; z < N; z++)
		{
			rowCheckList[z] = 0;
			colCheckList[z] = 0;
		}
		
		// check rows and cols
		for(int k = 0; k < N; k++)
		{
			rowSymbol = board[i][k];
			if(rowSymbol != EMPTY_FLAG)
			{
				rowCheckList[rowSymbol]++;
				if(rowCheckList[rowSymbol] > 1)
				{
					cout << "Violation: row " << (i+1) << " - placement of extra " << convertToPrintSymbol(rowSymbol) << endl;
					numViolate++;
					win = false;
				}
			}
			
			colSymbol = board[k][i];
			if(colSymbol != EMPTY_FLAG)
			{
				colCheckList[colSymbol]++;
				if(colCheckList[colSymbol] > 1)
				{
					cout << "Violation: column " << (i+1) << " - placement of extra " << convertToPrintSymbol(colSymbol) << endl;
					numViolate++;
					win = false;
				}
			}
		}
	}

	return win;
}

bool SChecker::check(string boardString)
{
	init(boardString);
	return check();
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