#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>

using namespace std;

int numNodes;   
double alpha = 1, beta = 1;
double initialPheromone = 0.2;
double numIterations = 3;

struct AntPath {
	vector<int> nodes;
	double totalDistance = 0;
};

struct Ant {
	vector<int> notVisited;
	AntPath antPath;
	int currentPosition;

	Ant(int startPosition) : currentPosition(startPosition) {
		for (int i = 0; i < numNodes; i++)
			notVisited.push_back(i);
		visitNode(startPosition);
		antPath.nodes.push_back(startPosition);
	}

	void visitNode(int node) {
		auto it = find(notVisited.begin(), notVisited.end(), node);
		if (it != notVisited.end()) {
			notVisited.erase(it);
		}
	}

	void printPath() const {
		cout << "Маршрут муравья: ";
		for (auto node : antPath.nodes) {
			cout << node << ' ';
		}
		cout << "расстояние: " << antPath.totalDistance << endl;
	}
};

struct TransitionStep {
	int from;
	int to;
	double probability;
};

struct CityEdge {
	double pheromone;       
	double distance;        

	CityEdge(double pheromone, double distance)
		: pheromone(pheromone), distance(distance) {}

	CityEdge() : pheromone(0), distance(0){}
};

struct Graph {
	const double evaporationRate = 0.64;
	const int size;
	AntPath bestPath{ {}, 9999 };
	vector<vector<CityEdge>> edges;

	Graph(int size) : size(size), edges(size, vector<CityEdge>(size)) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dist(1, 100);
		for (int i = 0; i < size; ++i) {
			edges[i].resize(size);
			for (int j = i + 1; j < size; ++j) {
				double distance = dist(gen);
				edges[i][j] = CityEdge(initialPheromone, distance);
				edges[j][i] = CityEdge(initialPheromone, distance);
			}
		}
	}

	void printGraph() const {
		cout << "Граф:" << endl;
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				cout << edges[i][j].distance << "  ";
				if (edges[i][j].distance < 10)
					cout << " ";
			}
			cout << endl;
		}
	}

	void updatePheromones() {
		for (auto& vec : edges) {
			for (auto& edge : vec) {
				edge.pheromone *= evaporationRate;
			}
		}
	}

	void addPheromones(const Ant& ant) {
		double pheromoneContribution = numNodes / ant.antPath.totalDistance;
		for (size_t i = 0; i < ant.antPath.nodes.size() - 1; i++) {
			int from = ant.antPath.nodes[i];
			int to = ant.antPath.nodes[i + 1];
			edges[from][to].pheromone += pheromoneContribution;
			edges[to][from].pheromone += pheromoneContribution;
		}
	}
};

void calculateDistance(Graph& graph, Ant& ant) {
	ant.antPath.totalDistance = 0;
	for (size_t i = 0; i < ant.antPath.nodes.size() - 1; i++) {
		int from = ant.antPath.nodes[i];
		int to = ant.antPath.nodes[i + 1];
		ant.antPath.totalDistance += graph.edges[from][to].distance;
	}
}

vector<TransitionStep> calculateProbabilities(const Graph& graph, const Ant& ant) {
	vector<TransitionStep> transitionSteps;
	double probabilitySum = 0;
	for (auto node : ant.notVisited) {
		double probability = pow(graph.edges[ant.currentPosition][node].pheromone, alpha)
			* pow(1 / graph.edges[ant.currentPosition][node].distance, beta);
		probabilitySum += probability;
	}
	for (auto node : ant.notVisited) {
		TransitionStep step;
		step.from = ant.currentPosition;
		step.to = node;
		step.probability = pow(graph.edges[ant.currentPosition][node].pheromone, alpha)
			* pow(1 / graph.edges[ant.currentPosition][node].distance, beta) / probabilitySum;
		transitionSteps.push_back(step);
	}

	return transitionSteps;
}

int selectNextNode(const vector<TransitionStep>& transitionSteps) {
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> dis(0.0, 1.0);

	double cumulativeProbability = 0;
	double randomValue = dis(gen);
	for (auto step : transitionSteps) {
		cumulativeProbability += step.probability;
		if (cumulativeProbability >= randomValue) {
			return step.to;
		}
	}
	return transitionSteps.front().to;
}

void updateBestPath(const vector<AntPath>& paths, AntPath& bestPath) {
	for (const auto& path : paths) {
		if (path.totalDistance < bestPath.totalDistance) {
			bestPath = path;
		}
	}
}

int main() {
	setlocale(LC_ALL, "rus");
	cout << "Введите количество узлов (N): ";
	cin >> numNodes;

	cout << "Введите начальное значение феромонов: ";
	cin >> initialPheromone;
	cout << "Введите Alpha: ";
	cin >> alpha;
	cout << "Введите Beta: ";
	cin >> beta;
	cout << "Введите количество итераций: ";
	cin >> numIterations;
	cout << endl;
	Graph graph(numNodes);
	graph.printGraph();
	cout << endl;
	for (size_t iter = 0; iter < numIterations; iter++) {
		graph.updatePheromones();
		vector<AntPath> antPaths;
		for (int antID = 0; antID < numNodes; antID++) {
			Ant ant(antID);
			for (int antMove = 0; antMove < numNodes - 1; antMove++) {
				vector<TransitionStep> transitionSteps = calculateProbabilities(graph, ant);
				int nextNode = selectNextNode(transitionSteps);
				ant.antPath.nodes.push_back(nextNode);
				ant.visitNode(nextNode);
				ant.currentPosition = ant.antPath.nodes.back();
			}
			ant.antPath.nodes.push_back(ant.antPath.nodes.front());
			calculateDistance(graph, ant);
			antPaths.push_back(ant.antPath);
			graph.addPheromones(ant);
		}
		updateBestPath(antPaths, graph.bestPath);
		cout <<"Итерация " << iter + 1 << ". Лучший маршрут: ";
		for (auto node : graph.bestPath.nodes) {
			cout << node << ' ';
		}
		cout << "(Расстояние = " << graph.bestPath.totalDistance << ")\n";
	}

	return 0;
}
