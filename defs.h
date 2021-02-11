#ifndef SUDOKU_CONST_H
#define SUDOKU_CONST_H

/************************/
/****** Constants *******/
const int N = 9;
const int NUM_SPACES = N*N;
const int NUM_COUSINS = N*3;
const int REGION_FACTOR = 3;
const int REGION_DIM = N / REGION_FACTOR;
const int X = 0;
const int Y = 1;
const int EMPTY_FLAG = -1;
const char EMPTY_SYMBOL = '.';

/************************/
/******* Classes ********/
class Space;
class SMaster;
struct Guess;

#endif