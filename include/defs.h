#ifndef SUDOKU_CONST_H
#define SUDOKU_CONST_H

/************************/
/****** Constants *******/
const int REGION_DIM = 3;
const int N = REGION_DIM * REGION_DIM;
const int NUM_SPACES = N*N;
const int NUM_COUSINS = N*3;
const int X = 0;
const int Y = 1;
const int EMPTY_FLAG = -1;
const char EMPTY_SYMBOL = '.';

/************************/
/******* Classes ********/
class Space;
class SMaster;
class SChecker;
class SGen;
struct Guess;

#endif