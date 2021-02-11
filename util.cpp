#include "util.h"

using namespace std;

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