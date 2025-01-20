#include <iostream>
#include <vector>
#include <queue>
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

// ��������� ��� �������� ����
struct Edge {
	int vertex; // ����� �������
	int weight; // ��� �����

	// �������� ��� ��������� (��� min-heap)
	bool operator>(const Edge& other) const {
		return weight > other.weight;
	}
};

void primMST() {
	vector<bool> inMST(SIZE, false);    // �����������, ����� ������� � MST
	priority_queue<Edge, vector<Edge>, greater<Edge>> pq; // ���-����

	// �������� � ������� 0
	pq.push({ 0, 0 });

	while (!pq.empty()) {
		// ��������� ������� � ����������� �����
		Edge current = pq.top();
		pq.pop();

		int v = current.vertex;

		// ���������� ��� ���������� �������
		if (inMST[v]) continue;

		inMST[v] = true; // ��������� ������� � MST

		// ������� ������� �����
		if (current.weight > 0)
			cout << "Vertex: " << v + 1 << ", Weight: " << current.weight << endl;

		// ��������� ��� �������� ������� � �������
		for (int i = 0; i < SIZE; i++) {
			if (!inMST[i] && GRAPH[v][i] > 0) {
				pq.push({ i, GRAPH[v][i] });
			}
		}
	}
}

int main() {
	primMST();
	return 0;
}
