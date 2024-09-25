#include <iostream>
#include <queue>
#include <stack>

using namespace std;

const int NODE_COUNT = 10;

int adjacencyMatrix[NODE_COUNT][NODE_COUNT] =
{
	{0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 0, 0, 0, 1, 0},
	{0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 1, 1, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
};

bool* visitedNodes = new bool[NODE_COUNT];

void depthFirstSearch(int node)
{
	static int nodesPrinted = 0;

	stack<int> nodes;
	nodes.push(node);
	visitedNodes[node] = true;

	while (!nodes.empty())
	{
		node = nodes.top();
		nodes.pop();

		cout << node + 1 << (nodesPrinted++ < NODE_COUNT - 1 ? " > " : "");

		for (int neighbor = NODE_COUNT - 1; neighbor >= 0; neighbor--)
		{
			if (adjacencyMatrix[node][neighbor] == 1 && !visitedNodes[neighbor])
			{
				nodes.push(neighbor);
				visitedNodes[neighbor] = true;
			}
		}
	}
}

void breadthFirstSearch(int startNode)
{
	static int nodesPrinted = 0;

	queue<int> nodeQueue;
	bool visited[NODE_COUNT] = { false };
	bool inQueue[NODE_COUNT] = { false };

	nodeQueue.push(startNode);
	visited[startNode] = inQueue[startNode] = true;

	while (!nodeQueue.empty())
	{
		int currentNode = nodeQueue.front();
		nodeQueue.pop();

		cout << currentNode + 1 << (nodesPrinted++ < NODE_COUNT - 1 ? " > " : "");

		for (int neighbor = 0; neighbor < NODE_COUNT; neighbor++)
		{
			if (!inQueue[neighbor] && adjacencyMatrix[currentNode][neighbor])
			{
				nodeQueue.push(neighbor);
				inQueue[neighbor] = true;
			}
		}
	}
}

void printAdjacencyMatrix()
{
	cout << "\nМатрица смежности графа:\n\n";
	for (int i = 0; i < NODE_COUNT; i++)
	{
		visitedNodes[i] = false;
		for (int j = 0; j < NODE_COUNT; j++)
		{
			cout << adjacencyMatrix[i][j] << " ";
		}
		cout << endl;
	}
}

void printEdgeList()
{
	cout << "\nСписок рёбер:\n\n";
	int edgeList1[] = { 1, 1, 2, 2, 3, 4, 4, 5, 6, 7, 9 };
	int edgeList2[] = { 2, 5, 7, 8, 8, 6, 9, 6, 9, 8, 10 };

	for (int i = 0; i < 11; i++)
	{
		cout << "{" << edgeList1[i] << ", " << edgeList2[i] << "}";
		cout << "{" << edgeList2[i] << ", " << edgeList1[i] << "}\n";
	}
}

void printAdjacencyList()
{
	cout << "\nСписок смежности:\n\n";
	int adjacencyList[NODE_COUNT][NODE_COUNT] =
	{
		{2, 5}, {7, 8}, {8}, {6, 9}, {1, 6}, {4, 5, 9}, {2, 8}, {2, 3, 7}, {4, 6, 10}, {9}
	};

	for (int i = 0; i < NODE_COUNT; i++)
	{
		cout << i + 1 << " -> ";
		for (int j = 0; adjacencyList[i][j] != 0; j++)
		{
			cout << adjacencyList[i][j] << " ";
		}
		cout << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "Rus");

	printEdgeList();

	cout << "\n\n";
	printAdjacencyMatrix();

	cout << "\n\n";
	printAdjacencyList();

	cout << "\nПоиск в глубину\n";
	int dfsStartNode;
	cout << "Введите начальную вершину: ";
	cin >> dfsStartNode;

	if (dfsStartNode > NODE_COUNT || dfsStartNode < 0)
	{
		cout << "Вершина должна быть от 1 до " << NODE_COUNT << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Порядок обхода: ";
	depthFirstSearch(dfsStartNode - 1);
	cout << endl;

	cout << "\nПоиск в ширину\n";
	int bfsStartNode;
	cout << "Введите начальную вершину: ";
	cin >> bfsStartNode;

	if (bfsStartNode > NODE_COUNT || bfsStartNode < 0)
	{
		cout << "Вершина должна быть от 1 до " << NODE_COUNT << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Порядок обхода: ";
	breadthFirstSearch(bfsStartNode - 1);
	cout << endl;

	delete[] visitedNodes;

	return 0;
}
