#include <iostream>

using namespace std;

constexpr int SIZE = 9;

int graph[SIZE][SIZE] = {
	/*A*/{0,  7,  10, 0,  0,  0,  0,  0,   0},
	/*B*/{7,  0,  0,  0,  0,  9,  27, 0,   0},
	/*C*/{10, 0,  0,  0,  31, 8,  0,  0,   0},
	/*D*/{0,  0,  0,  0,  32, 0,  0,  17, 21},
	/*E*/{0,  0,  31, 32, 0,  0,  0,  0,   0},
	/*F*/{0,  9,  8,  0,  0,  0,  0,  11,  0},
	/*G*/{0,  27, 0,  0,  0,  0,  0,  0,  15},
	/*H*/{0,  0,  0,  17, 0,  11, 0,  0,  15},
	/*I*/{0,  0,  0,  21, 0,  0,  15, 15,  0},
};

int FindMinDistIndex(const int* dist, const bool* visited)
{
	int minDist = INT32_MAX;
	int minIndex = -1;

	for (int node = 0; node < SIZE; node++)
	{
		if (!visited[node] && dist[node] < minDist)
		{
			minDist = dist[node];
			minIndex = node;
		}
	}
	return minIndex;
}

void DijkstraAlgorithm(int startNode)
{
	int distances[SIZE];
	bool visited[SIZE];

	for (int i = 0; i < SIZE; ++i)
	{
		distances[i] = INT32_MAX;
		visited[i] = false;
	}

	distances[startNode] = 0;

	for (int count = 0; count < SIZE; count++)
	{
		int minDist = INT32_MAX;
		int minIndex = FindMinDistIndex(distances, visited);

		for (int node = 0; node < SIZE; node++)
		{
			if (!visited[node] && distances[node] < minDist)
			{
				minDist = distances[node];
				minIndex = node;
			}
		}

		if (minIndex == -1)
		{
			break;
		}

		visited[minIndex] = true;

		for (int node = 0; node < SIZE; node++)
		{
			if (!visited[node] && graph[node][minIndex] && distances[minIndex] + graph[node][minIndex] < distances[node])
			{
				distances[node] = distances[minIndex] + graph[minIndex][node];
			}
		}
	}

	cout << "\nМинимальные расстояния:\n";
	for (int i = 0; i < SIZE; i++)
	{
		cout << (char)(startNode + 'A') << " -> " << (char)(i + 'A') << " = " << distances[i] << endl;
	}
}

int charToIndex(char node) {

	node = toupper(node);
	if (node >= 'A' && node <= 'I') {
		return node - 'A';
	}
	return -1;
}

int main() {

	setlocale(LC_ALL, "Ru");

	char node;
	int startVertex;

	cout << "Введите вершину A-I): ";
	cin >> node;
	startVertex = charToIndex(node);
	if (startVertex == -1)
	{
		cout << "Неверная вершина\n";
		return 0;
	}

	DijkstraAlgorithm(startVertex);

	return 0;
}
