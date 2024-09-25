#include <Windows.h>
#include <iostream>
#include <initializer_list>
#include <queue>
#include <vector>
#include <unordered_set>

using namespace std;

struct Node
{
	int value;
	int rearsAmount;
	int* rears = nullptr;

	Node(int val, std::initializer_list<int> rearsList)
		: value(val), rearsAmount(rearsList.size())
	{
		std::vector<int> temp(rearsList);
		std::sort(temp.begin(), temp.end());
		rears = new int[rearsAmount];

		int i = 0;
		for (int rear : temp)
		{
			rears[i++] = rear - 1;
		}
	}

	Node() {}

	// Оператор копирования
	Node& operator=(const Node& other)
	{
		if (this != &other)
		{
			delete[] rears;

			value = other.value;
			rearsAmount = other.rearsAmount;
			rears = new int[rearsAmount];
			for (int i = 0; i < rearsAmount; ++i)
			{
				rears[i] = other.rears[i];
			}
		}
		return *this;
	}

	~Node()
	{
		delete[] rears;
	}
};

// BFS функция, работающая с указателями на узлы
vector<int> BFS(Node** nodes);

//int main() {
//	SetConsoleOutputCP(1251);
//
//	Node** nodes = new Node * [10];
//	nodes[0] = new Node(1, { 2, 5 });
//	nodes[1] = new Node(2, { 1, 7, 8 });
//	nodes[2] = new Node(3, { 8 });
//	nodes[3] = new Node(4, { 6, 9 });
//	nodes[4] = new Node(5, { 1, 6 });
//	nodes[5] = new Node(6, { 4, 5, 9 });
//	nodes[6] = new Node(7, { 1, 8 });
//	nodes[7] = new Node(8, { 2, 3, 7 });
//	nodes[8] = new Node(9, { 4, 6, 10 });
//	nodes[9] = new Node(10, { 9 });
//
//
//	auto res = BFS(nodes);
//
//	for (auto c : res)
//	{
//		cout << c << " ";
//	}
//
//	// Освобождаем память
//	for (int i = 0; i < 8; ++i)
//	{
//		delete nodes[i];
//	}
//	delete[] nodes;
//
//	return 0;
//}

vector<int> BFS(Node** nodes)
{
	vector<int> result;  // Вектор для хранения результата в порядке вставки
	unordered_set<int> visited;  // Набор для отслеживания уникальных элементов
	queue<Node*> q;

	q.push(nodes[0]); // Добавляем начальный узел

	while (!q.empty())
	{
		Node* node = q.front();  // Извлекаем элемент из начала очереди
		q.pop();

		// Если узел уже был посещён, пропускаем его
		if (visited.find(node->value) != visited.end())
		{
			continue;
		}

		// Добавляем текущий узел в результат
		result.push_back(node->value);
		visited.insert(node->value);  // Отмечаем узел как посещённый

		// Проходим по всем соседним узлам
		for (int i = 0; i < node->rearsAmount; ++i)
		{
			int rearIdx = node->rears[i];
			if (visited.find(nodes[rearIdx]->value) == visited.end())
			{
				q.push(nodes[rearIdx]);  // Добавляем в очередь непосещённый узел
			}
		}
	}
	return result;
}