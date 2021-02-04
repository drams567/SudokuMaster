#include <iostream>

using namespace std;

const int N = 9;
const int NUM_SPACES = N*N;
const int REGION_FACTOR = 3;
const int REGION_DIM = N / REGION_FACTOR;

void listCousins(int x, int y)
{
	cout << "Region size: " << REGION_DIM << endl;
	int startX;
	if(x == 0)
		startX = x;
	else
		startX = x - (x % REGION_DIM);
	
	int startY;
	if(y == 0)
		startY = y;
	else
		startY = y - (y % REGION_DIM);
	
	int endX = startX + REGION_DIM;
	int endY = startY + REGION_DIM;
	for(int i = startX; i < endX; i++)
	{
		for(int k = startY; k < endY; k++)
		{
			cout << i << "," << k << endl;
		}
	}
}	

int main()
{
	int index[] = {5, 5};
	listCousins(index[0], index[1]);
	
	return 0;
}