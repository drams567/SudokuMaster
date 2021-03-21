#include "util.h"

using namespace std;

/*********************/
/* Utility Functions */
int convertToSymbol(const char charSymbol)
{
	if(charSymbol >= '0')
		return (int)(charSymbol - '0') - 1;
	else
		return EMPTY_FLAG;
}

char convertToPrintSymbol(const int inSymbol)
{
	if(inSymbol == EMPTY_FLAG)
		return EMPTY_SYMBOL;
	else
		return ('0' + inSymbol) + 1; // literal acts as an offset
}

void printBoard(const string boardString)
{
	cout << endl;
	cout << string(N*3 + 5, '-') << endl;
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			if(k % 3 == 0)
				cout << "|";
			
			cout << " " << boardString[(i*N) + k] << " ";
		}
		cout << " |" << endl;
		if(i % 3 == 2)
			cout << string(N*3 + 5, '-') << endl;
		else
			cout << endl;
	}
	
	cout << endl;
}