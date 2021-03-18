#include <iostream>
#include <fstream>
#include <string>

#include "sudokuMaster.h"
#include "sudokuChecker.h"
#include "sudokuGen.h"

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

	/*
	SMaster SM(boardString);
	SM.dumpBoard();
	cout << endl;
	SM.solve();
	SM.dumpBoard();
	cout << endl;
	
	//boardString = SM.getBoard();
	*/
   
	/*
	cout << "TESTING SChecker" << endl;
	SChecker SC;
	bool ret = SC.check(boardString);
	SC.dumpResults();

	//SM.dumpStats();
	*/

	/*
	SMaster SM(boardString);
	SM.dumpBoard();
	cout << endl;
	if(SM.checkRegionState(6,6))
	{
		cout << "Region good" << endl;
	}
	else
	{
		cout << "Region bad" << endl;
	}
	*/

	unsigned int testSeed = 12344321;
	SGen Generator(testSeed);
	SChecker Checker;
	SMaster Solver;
	int result = 1;
	int i = 0;
	int numRuns = 4923;
	int boardSize = 10;
   /*
	do
	{
		Solver.solve(Generator.genBoard(boardSize));
		result = Checker.check(Solver.getBoard());
		if(result < 0)
		{
			cout << "MAIN: Failure, " << Checker.getNumViolate() << " violations found" << endl;
		}
      
		if(i > 4800)
		{
			cout << i << " complete" << endl;
		}

		i++;
      
	} while(result != -1 && i <= numRuns);
   */
   
	printBoard(boardString);
	SMaster tester(boardString);
	tester.testSolve();

	cout << endl << "Enter any key to exit..." << endl;
	getchar();

	return 0;
}