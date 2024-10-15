#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const int SIZE = 8;

const int GRAPH[SIZE][SIZE] = {
	{0, 2,  0,  8,  2,  0,  0, 0},
	{2, 0,  3,  10, 5,  0,  0, 0},
	{0, 3,  0,  0,  12, 0,  0, 7},
	{8, 10, 0,  0,  14, 3,  1, 0},
	{2, 5,  12, 14, 0,  11, 4, 8},
	{0, 0,  0,  3,  11, 0,  6, 0},
	{0, 0,  0,  1,  4,  6,  0, 9},
	{0, 0,  7,  0,  8,  0,  9, 0},
};

void primMST()
{
	vector<bool> inMST(SIZE, false);
	inMST[0] = true;

	for (int edges = 0; edges < SIZE - 1; edges++)
	{
		int min = INT_MAX;
		int x = -1, y = -1;

		for (int i = 0; i < SIZE; i++)
		{
			if (inMST[i]) {
				for (int j = 0; j < SIZE; j++)
				{
					if (!inMST[j] && GRAPH[i][j] && GRAPH[i][j] < min)
					{
						min = GRAPH[i][j];
						x = i;
						y = j;
					}
				}
			}
		}

		if (x != -1 && y != -1) {
			cout << (x + 1) << " - " << (y + 1) << " == " << GRAPH[x][y] << endl;
			inMST[y] = true;
		}
	}
}

int main()
{
	primMST();
	return 0;
}
