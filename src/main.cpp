#include <iostream>
#include <fstream>
#include <string>

#include "sudokuMaster.h"

using namespace std;

int main(const int argc, const char* argv[])
{	
	// ARGS: run, board string
	if(argc < 2)
	{
		cout << endl << "main.cpp: Missing argument(s). Exiting...." << endl;
		exit(0);
	}
	
	string boardFileName = argv[1];
	if(boardFileName.length() < 5 || boardFileName.substr(boardFileName.length() - 4) != ".txt")
	{
		cout << endl << "main.cpp: Board file is invalid. Exiting..." << endl;
		exit(0);
	}
	
	ifstream boardStream(boardFileName);
	string boardString;
	boardStream >> boardString;
	boardStream.close();

	if(boardString.length() != NUM_SPACES)
	{
		cout << endl << "main.cpp: Board string is invalid. Exiting..." << endl;
		exit(0);
	}
	
	SMaster SM(boardString);
	SM.dumpBoard();
	SM.solve();
	SM.dumpBoard();
	
	string test = SM.getBoard();
	cout << "\nBoard String: " << test << "\n";
	
	return 0;
}