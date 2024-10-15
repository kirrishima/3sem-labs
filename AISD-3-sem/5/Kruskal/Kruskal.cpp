#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
	int u, v, weight;

	bool operator<(const Edge& other) const {
		return weight < other.weight;
	}
};

vector<int> parent;

int find(int u) {
	while (u != parent[u])
	{
		u = parent[u];
	}
	return parent[u];
}

void unite(int u, int v) {
	int root_u = find(u);
	int root_v = find(v);

	if (root_u != root_v) {
		parent[root_u] = root_v;
	}
}


vector<Edge> kruskal(vector<Edge>& edges, int n) {
	vector<Edge> mst;

	sort(edges.begin(), edges.end());

	parent.resize(n);
	for (int i = 0; i < n; ++i) {
		parent[i] = i;
	}

	for (const auto& edge : edges) {
		if (find(edge.u) != find(edge.v)) {
			mst.push_back(edge);
			unite(edge.u, edge.v);
		}
	}

	return mst;
}

int main() {
	setlocale(LC_ALL, "ru");

	vector<Edge> edges = {
		{0, 1, 2},
		{0, 3, 8},
		{0, 4, 2},
		{1, 3, 10},
		{1, 2, 3},
		{1, 4, 5},
		{2, 7, 7},
		{2, 4, 12},
		{4, 7, 7},
		{4, 3, 14},
		{4, 5, 11},
		{4, 6, 4},
		{3, 5, 3},
		{3, 6, 1},
		{5, 6, 6},
		{6, 7, 9},
	};

	vector<Edge> mst = kruskal(edges, 8);

	cout << "Минимальное остовное дерево содержит рёбра:\n";
	for (const auto& edge : mst) {
		cout << "Ребро: " << edge.u + 1 << " - " << edge.v + 1 << " с весом " << edge.weight << endl;
	}
	return 0;
}