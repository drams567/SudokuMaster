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

	unsigned int testSeed = 12344321;
	SGen Generator(testSeed);
	SChecker Checker;
	SMaster Solver;
	int result = 1;
	int i = 0;
	int numRuns = 10000;
	int boardSize = 10;
	int totalNumGuess = 0;
	int totalNumBadGuess = 0;
	int totalNumMoves = 0;
	do
	{
		Solver.solve(Generator.genBoard(boardSize));

		result = Checker.check(Solver.getBoard());
		if(result == 1)
		{
			totalNumGuess += Solver.getNumGuess();
			totalNumBadGuess += Solver.getNumBadGuess();
			totalNumMoves += Solver.getNumMoves();
		}
		else if(result == 0)
		{
			cout << "MAIN: Run " << (i+1) << " failed, " << Checker.getNumMissing() << " spaces missing." << endl;
		}
		else
		{
			cout << "MAIN: Run " << (i+1) << " failed, " << Checker.getNumViolate() << " violations found." << endl;
		}


		if(i % (numRuns/10) == 0)
		{
			cout << i << " complete" << endl;
		}
		i++;
      
	} while(result != -1 && i <= numRuns);
	
	double avgNumGuess = (double)totalNumGuess / (double)numRuns;
	double avgNumBadGuess = (double)totalNumBadGuess / (double)numRuns;
	double avgNumMoves = (double)totalNumMoves / (double)numRuns;

	cout << endl << "----Averages----" << endl;
	cout << "Guesses:\t" << avgNumGuess << " (" << avgNumBadGuess << " of them bad on avg)" << endl;
	cout << "Moves:\t\t" << avgNumMoves << endl;

	return 0;
}