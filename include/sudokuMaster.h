#ifndef SUDOKU_MASTER_H
#define SUDOKU_MASTER_H

#include <iostream>
#include <vector>
#include <string>
#include "util.h"
#include "space.h"
#include "defs.h"

using namespace std;

/*********************/
/****** Classes ******/
struct Guess {
	Space* guessSpace;		// Guess Information members
	int guessSymbol;		// ^
	Space boardState[N][N];				// Game state information members
	Space* remainListState[NUM_SPACES];	// ^
	int numRemainState;					// ^
};

class SMaster {
protected:
	Space board[N][N];				// Sudoku board
	Space* remainList[NUM_SPACES];	// Remaining spaces to solve
	int numRemain;					// Number of remaining spaces to solve
	Guess guessList[NUM_SPACES];	// Stack of guess moves currently taken
	int numGuess;					// Number of guess moves currently taken
	
	int statNumBadGuess;			// Number of guesses taken that were corrected
	int statNumGuess;				// Number of guesses taken
	int statNumMoves;				// Number of moves taken during solving (including unecessarily taken moves)
	
public:
	// Construct and Destruct //
	SMaster();
	SMaster(const string boardString);
	~SMaster();
	
	// Solve Board //
	void solve();
	void solve(const string boardString);
	void testSolve();

	// Procedures //
	void init(const string boardString);
	void makeGuess();
	void makeMove(Space* space, bool isGuess);
	void cullReserved(const int startX, const int startY);
	void recallLastGuess();
	void saveStateAsGuess(Space* space, int move);

	// Helpers //
	int getBestSpaceIndex();
	vector<Space*> getCousins(Space* inSpace);
	bool checkRegionState(const int startX, const int startY);
	
	// List Functions //
	void pushRemain(const int x, const int y);
	void pushRemain(Space* inSpace);
	void popRemain();
	void removeRemain(const int i);
	void pushGuess(Guess newGuess);
	Guess popGuess();
	
	// Setters and Getters //
	string getBoard();
	int getNumGuess();
	int getNumBadGuess();
	int getNumMoves();
	
	// Debugging //
	void dumpBoard();
	void dumpSpaces();
	void dumpRemain();
	void dumpStats();
};

#endif
