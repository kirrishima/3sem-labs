#include <iostream>

using namespace std;

#define INF INT_MAX

void ComputeShortestPaths();
void DisplayMatricesAsTables();


int D[6][6]
{
		{0, 28,  21,   59,   12,  27},
		{7, 0,   24,   INF,  21,  9},
		{9, 32,  0,    13,   11,  INF},
		{8, INF, 5,    0,    16,  INF},
		{14, 13, 15,   10,   0,   22},
		{15, 18, INF,  INF,  6,   0}
};

int S[6][6]
{
		{0, 2, 3, 4, 5, 6},
		{1, 0, 3, 4, 5, 6},
		{1, 2, 0, 4, 5, 6},
		{1, 2, 3, 0, 5, 6},
		{1, 2, 3, 4, 0, 6},
		{1, 2, 3, 4, 5, 0}
};

int main()
{
	setlocale(LC_ALL, "Ru");

	ComputeShortestPaths();
	DisplayMatricesAsTables();

	return 0;
}


void ComputeShortestPaths()
{
	for (int m = 0; m < 6; m++)
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if (D[m][i] != INF && D[m][j] != INF && D[i][m] + D[m][j] < D[i][j])
				{
					D[i][j] = D[i][m] + D[m][j];
					S[i][j] = S[i][m];
				}
			}
		}
	}
}


void DisplayMatricesAsTables()
{
	cout << "Матрица расстояний:\n";
	cout << "\n|------|------|------|------|------|------|\n";

	for (int i = 0; i < 6; i++)
	{
		cout << "|";
		for (int j = 0; j < 6; j++)
		{
			if (D[i][j] == INF)
				cout << " INF |";
			else
				printf("%4d  |", D[i][j]);
		}
		cout << "\n|------|------|------|------|------|------|\n";
	}


	cout << "\nМатрица путей:\n";
	cout << "\n|------|------|------|------|------|------|\n";

	for (int i = 0; i < 6; i++)
	{
		cout << "|";
		for (int j = 0; j < 6; j++)
		{
			printf("%4d  |", S[i][j]);
		}
		cout << "\n|------|------|------|------|------|------|\n";
	}
}
