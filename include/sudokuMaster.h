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
	int guessSymbol;		//
	Space boardState[N][N];				// Game state information members
	Space* remainListState[NUM_SPACES];	//
	int numRemainState;					//
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
	SMaster(string boardString);
	~SMaster();
	
	// Solve Board //
	void solve();
	
	// Procedures //
	void makeGuess();
	void makeMove(Space* space, bool isGuess);
	void restoreFromGuess(Guess badGuess);
	void cullReserved(int startX, int startY);
	
	// Helpers //
	int getBestSpaceIndex();
	vector<Space*> getCousins(Space* inSpace);
	bool checkRegionState(int startX, int startY);
	
	// List Functions //
	void pushRemain(int x, int y);
	void pushRemain(Space* inSpace);
	void popRemain();
	void removeRemain(int i);
	void pushGuess(Guess newGuess);
	Guess popGuess();
	
	// Setters and Getters //
	string getBoard();
	
	// Debugging //
	void dumpBoard();
	void dumpSpaces();
	void dumpRemain();
	void dumpStats();
};

#endif
