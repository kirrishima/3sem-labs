#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>

using namespace std;

int N;
double alpha = 1, beta = 1;
double startFeromones = 0.2, iterations = 3;

struct Path {
    vector<int> path;
    double distance = 0;
};

struct Ant {
    vector<int> notVisited;
    Path antPath;
    int currentPosition;

    Ant(int startPosition) : currentPosition(startPosition) {
        for (int i = 0; i < N; i++)
            notVisited.push_back(i);
        visiteNode(startPosition);
        antPath.path.push_back(startPosition);
    }

    void visiteNode(int Node) {
        auto it = find(notVisited.begin(), notVisited.end(), Node);
        if (it != notVisited.end()) {
            notVisited.erase(it);
        }
    }

    void PrintPath() {
        cout << "Маршрут муравья: ";
        for (auto node : antPath.path) {
            cout << node << ' ';
        }
        cout << "расстояние: " << antPath.distance << endl;
    }
};

struct Step {
    int from;
    int to;
    double posibility;
};

struct GRAPH {
    const double evaporation = 0.64;
    const int SIZE;
    Path bestPath{ {}, 9999 };
    vector<vector<pair<double, double>>> gr;

    GRAPH(int size) : SIZE(size), gr(size, vector<pair<double, double>>(size)) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(1, 100);
        for (int i = 0; i < SIZE; ++i) {
            gr[i].resize(size);
            for (int j = i + 1; j < SIZE; ++j) {
                double distance = dist(gen);
                gr[i][j] = pair<double, double>(distance, startFeromones);
                gr[j][i] = pair<double, double>(distance, startFeromones);
            }
        }
    }

    void PrintGraph() {
        cout << "Граф:" << endl;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                cout << gr[i][j].first << "  ";
                if (gr[i][j].first < 10)
                    cout << " ";
            }
            cout << endl;
        }
    }

    void UpdateFeromones() {
        for (auto& vec : gr) {
            for (auto& node : vec) {
                node.second *= evaporation;
            }
        }
    }

    void AddFeromones(Ant& ant) {
        double antFeromone = N / ant.antPath.distance;
        for (int i = 0; i < ant.antPath.path.size() - 1; i++) {
            int from = ant.antPath.path[i];
            int to = ant.antPath.path[i + 1];
            gr[from][to].second += antFeromone;
            gr[to][from].second += antFeromone;
        }
    }
};

void calculateDistance(GRAPH& graph, Ant& ant) {
    ant.antPath.distance = 0;
    for (size_t i = 0; i < ant.antPath.path.size() - 1; i++) {
        int from = ant.antPath.path[i];
        int to = ant.antPath.path[i + 1];
        ant.antPath.distance += graph.gr[from][to].first;
    }
}

//расчет вероятностей переходов
vector<Step> calculatePossibilities(GRAPH& graph, Ant& ant) {
    vector<Step> StepPossibilities;
    double posibilitySum = 0;
    for (auto node : ant.notVisited) {
        double posibility = pow(graph.gr[ant.currentPosition][node].second, alpha) * pow(1 / graph.gr[ant.currentPosition][node].first, beta);
        posibilitySum += posibility;
    }
    for (auto node : ant.notVisited) {
        Step step;
        step.from = ant.currentPosition;
        step.to = node;
        step.posibility = pow(graph.gr[ant.currentPosition][node].second, alpha) * pow(1 / graph.gr[ant.currentPosition][node].first, beta) / posibilitySum;
        StepPossibilities.push_back(step);
    }

    return StepPossibilities;
}

//выбор следующей вершины
int findNextNode(vector<Step> Steps) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0f, 1.0f);

    double sumOfPossibilities = 0;
    double randKoef = dis(gen);
    for (auto Step : Steps) {
        if ((sumOfPossibilities += Step.posibility) >= randKoef) {
            return Step.to;
        }
    }
    return Steps.front().to;
}

void findBestPath(vector<Path>& pathes, Path& bestPath) {
    for (auto path : pathes) {
        if (path.distance < bestPath.distance)
            bestPath = path;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "Введите количество узлов (N): ";
    cin >> N;

    cout << "Введите начальное значение феромонов: ";
    cin >> startFeromones;
    cout << "Введите Alpha: ";
    cin >> alpha;
    cout << "Введите Beta: ";
    cin >> beta;
    cout << "Введите количество итераций: ";
    cin >> iterations;

    srand(time(0));
    GRAPH graph(N);
    graph.PrintGraph();

    cout << endl << endl;
    for (size_t iter = 0; iter < iterations; iter++) {
        graph.UpdateFeromones();
        vector<Path> AntsPaths;
        for (int antID = 0; antID < N; antID++) {
            Ant ant(antID);
            for (int antMove = 0; antMove < N - 1; antMove++) {
                vector<Step> PathPossibilities = calculatePossibilities(graph, ant);
                int nextNode = findNextNode(PathPossibilities);
                ant.antPath.path.push_back(nextNode);
                ant.visiteNode(nextNode);
                ant.currentPosition = ant.antPath.path.back();
            }
            ant.antPath.path.push_back(ant.antPath.path.front());
            calculateDistance(graph, ant);
            AntsPaths.push_back(ant.antPath);
            graph.AddFeromones(ant);
        }
        findBestPath(AntsPaths, graph.bestPath);
        cout << iter << "  Лучший маршрут: ";
        for (auto node : graph.bestPath.path) {
            cout << node << ' ';
        }
        cout << "расстояние: " << graph.bestPath.distance << endl;
    }

    return 0;
}