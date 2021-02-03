#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int N = 9;
const int NUM_SPACES = N*N;
const int X = 0;
const int Y = 1;

class Space {
private:
	int symbol;
	bool fixed;
	int index[2];

public:
	Space(int x, int y)
	{
		symbol = -1;
		fixed = false;
		index[X] = x;
		index[Y] = y;
	}
	
	Space(int x, int y, int newSymbol)
	{
		symbol = newSymbol;
		fixed = true;
		index[X] = x;
		index[Y] = y;
	}
	
	~Space(){}
};

void printBoard(string boardString)
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
			cout << " " << boardString[(i*N + k)] << " ";
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

	// print board
	printBoard(boardString);

	// Initialize board
	Space* board[N][N];
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			board[i][k] = new Space(i, k);
		}
	}
	
	// Clean board
	for(int i = 0; i < N; i++)
	{
		for(int k = 0; k < N; k++)
		{
			if(board[i][k] != NULL)
			{
				delete board[i][k];
				board[i][k] = NULL;
			}
		}
	}
	
	return 0;
}