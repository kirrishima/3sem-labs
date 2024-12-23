#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <Windows.h>
#include <time.h>

using namespace std;

int V = 5; // Количество городов
#define GENES ABCDE // Гены маршрута
#define START 0 // Начальный город

// Глобальные переменные для генетического алгоритма
int child = 0; // Количество потомков в каждом поколении
int populationSize = 0; // Размер популяции
int evoCount = 0; // Количество поколений
double mutationChance = 0; // Шанс мутации

// Матрица расстояний между городами
vector<vector<int>> cities = {
	{ INT_MAX, 25, 40, 31, 27 },
	{ 5, INT_MAX, 17, 30, 25 },
	{ 19, 15, INT_MAX, 6, 1 },
	{ 9, 50, 24, INT_MAX, 6 },
	{ 22, 8, 7, 10, INT_MAX }
};

// Структура для хранения маршрута и его длины
struct Path {
	string path;
	int lenght;
};

// Функция rand_num
// -----------------
// Цель: 
// Генерирует случайное число в указанном диапазоне [start, end).
//
// Алгоритм:
// 1. Вычисляется диапазон возможных значений: r = end - start.
// 2. Генерируется случайное число с помощью функции rand() в пределах от 0 до r-1.
// 3. К этому числу прибавляется start, чтобы сдвинуть диапазон, и результат возвращается.
//
// Переменные:
// - start: Нижняя граница диапазона (включительно).
// - end: Верхняя граница диапазона (исключительно).
// - r: Размер диапазона чисел.
// - rnum: Итоговое случайное число.
int rand_num(int start, int end) {
	int r = end - start;
	int rnum = start + rand() % r;
	return rnum;
}

// Функция contains
// -----------------
// Цель:
// Проверяет, содержится ли указанный символ `ch` в строке `s`.
//
// Алгоритм:
// 1. Инициализируется цикл по всем символам строки `s`.
// 2. На каждой итерации сравнивается текущий символ строки `s[i]` с искомым символом `ch`.
// 3. Если символ найден, возвращается true, иначе проверка продолжается.
// 4. Если цикл завершился без нахождения символа, возвращается false.
//
// Переменные:
// - s: Строка, в которой выполняется поиск.
// - ch: Символ, который нужно найти.
// - i: Индекс текущего символа в строке `s`.
bool contains(string s, char ch) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ch)  // Если текущий символ совпадает с искомым
			return true; // Символ найден
	}
	return false; // Символ не найден
}

// Функция mutatedGene
// --------------------
// Цель:
// Выполняет мутацию маршрута, меняя местами два случайных города.
//
// Алгоритм:
// 1. Запускается бесконечный цикл.
// 2. Выбираются два случайных индекса r и r1 (в пределах от 1 до V).
// 3. Если индексы r и r1 не совпадают:
//    - Обмениваются значения в строке path по индексам r и r1.
//    - Прерывается цикл, так как мутация завершена.
// 4. Возвращается новый изменённый маршрут.
//
// Переменные:
// - path: Исходный маршрут, представленный в виде строки.
// - r, r1: Случайные индексы в строке, выбираемые для обмена.
// - temp: Временная переменная для хранения символа при обмене.
string mutatedGene(string path) {
	while (true) {
		int r = rand_num(1, V);  // Генерируем первый случайный индекс
		int r1 = rand_num(1, V); // Генерируем второй случайный индекс
		if (r1 != r) {           // Проверяем, чтобы индексы не совпадали
			char temp = path[r]; // Сохраняем символ с позиции r
			path[r] = path[r1];  // Меняем местами символы
			path[r1] = temp;     // Завершаем обмен
			break;               // Прерываем цикл
		}
	}
	return path; // Возвращаем мутировавший маршрут
}

// Функция для сравнения двух маршрутов по длине
bool lessthan(struct Path t1, struct Path t2) {
	return t1.lenght < t2.lenght;
}

// Функция create_path
// --------------------
// Цель:
// Генерирует случайный маршрут для задачи коммивояжера.
//
// Алгоритм:
// 1. Инициализируется маршрут с первого города: path = "0".
// 2. Запускается цикл, пока длина маршрута не достигнет количества городов (V):
//    - Генерируется случайный город temp (от 1 до V-1).
//    - Проверяется, есть ли город temp в текущем маршруте (функция contains).
//    - Если города нет в маршруте, он добавляется в path.
// 3. Когда маршрут содержит все города, добавляется стартовый город (path[0]) для завершения цикла.
// 4. Возвращается сгенерированный маршрут.
//
// Переменные:
// - path: Генерируемый маршрут в виде строки.
// - temp: Случайный город, который добавляется в маршрут.
string create_path() {
	string path = "0"; // Начинаем маршрут с города 0
	while (true) {
		if (path.size() == V) { // Если маршрут содержит все города
			path += path[0];    // Замыкаем маршрут, возвращаясь в стартовый город
			break;              // Завершаем цикл
		}
		int temp = rand_num(1, V); // Генерируем случайный следующий город
		if (!contains(path, (char)(temp + 48))) // Если город еще не в маршруте
			path += (char)(temp + 48);          // Добавляем его
	}
	return path; // Возвращаем сгенерированный маршрут
}

// Функция calculate_lenght
// -------------------------
// Цель:
// Вычисляет общую длину маршрута на основе матрицы расстояний.
//
// Алгоритм:
// 1. Инициализируется переменная f для хранения общей длины маршрута.
// 2. Запускается цикл по всем парам соседних городов в маршруте:
//    - Извлекаются текущий город (path[i]) и следующий (path[i+1]).
//    - Если расстояние между этими городами в матрице равно INT_MAX, 
//      маршрут невозможен, возвращается INT_MAX.
//    - В противном случае расстояние между городами добавляется к f.
// 3. После завершения цикла возвращается значение f.
//
// Переменные:
// - path: Маршрут в виде строки, где каждый символ — индекс города.
// - f: Общая длина маршрута.
// - i: Индекс текущего города в маршруте.
int calculate_lenght(string path) {
	int f = 0; // Суммарная длина маршрута
	for (int i = 0; i < path.size() - 1; i++) {
		if (cities[path[i] - 48][path[i + 1] - 48] == INT_MAX)
			return INT_MAX; // Если расстояние между городами бесконечное
		f += cities[path[i] - 48][path[i + 1] - 48]; // Добавляем расстояние между городами
	}
	return f; // Возвращаем общую длину маршрута
}

// Функция childGene
// ------------------
// Цель:
// Создаёт нового потомка из двух маршрутов-родителей (генетический кроссовер).
//
// Алгоритм:
// 1. Выбирается случайная точка кроссовера r (от 1 до V-1).
// 2. Первая часть маршрута потомка (child.path) берётся из маршрута отца до точки r.
// 3. Вторая часть маршрута формируется из городов матери, которых ещё нет в маршруте потомка.
// 4. Если маршрут потомка ещё не содержит всех городов, добавляются недостающие города из маршрута отца.
// 5. Добавляется начальный город для завершения маршрута (замыкание цикла).
// 6. Вычисляется длина маршрута потомка с помощью calculate_lenght.
// 7. Возвращается сформированный маршрут потомка.
//
// Переменные:
// - dad, mom: Родители (структуры Path), из которых создаётся потомок.
// - child: Потомок (структура Path).
// - r: Случайная точка кроссовера.
Path childGene(Path dad, Path mom) {
	Path child;
	int r = rand_num(1, V); // Случайная точка кроссовера
	child.path = dad.path.substr(0, r); // Первая часть от отца
	string mom_path = mom.path.substr(r + 1); // Вторая часть от матери
	for (char ch : mom_path) {
		if (!contains(child.path, ch)) { // Добавляем города от матери, которых ещё нет
			child.path += ch;
		}
	}
	// Если есть недостающие города, добавляем их от отца
	if (child.path.size() != V) {
		for (int i = r; i < dad.path.size(); i++) {
			if (!contains(child.path, dad.path[i])) {
				child.path += dad.path[i];
			}
		}
	}
	child.path += child.path[0]; // Замыкаем маршрут
	child.lenght = calculate_lenght(child.path); // Вычисляем длину маршрута
	return child; // Возвращаем потомка
}

// Функция addCity
// ----------------
// Цель:
// Добавляет новый город в матрицу расстояний между городами.
//
// Алгоритм:
// 1. Создаются два вектора:
//    - newDistances: Расстояния от нового города до существующих городов.
//    - newDistances2: Расстояния от существующих городов до нового города.
// 2. Пользователю предлагается ввести расстояния между новым городом и другими городами:
//    - Сначала вводятся расстояния "к" каждому городу.
//    - Затем вводятся расстояния "от" каждого города.
// 3. Расстояния от нового города добавляются в существующие строки матрицы (vector<vector<int>>).
// 4. Строка newDistances, включающая саму себя (INT_MAX), добавляется как новая строка в матрицу.
// 5. Увеличивается общее количество городов (V).
//
// Переменные:
// - newDistances: Расстояния от нового города до остальных.
// - newDistances2: Расстояния от остальных городов до нового города.
// - V: Количество городов в матрице.
void addCity() {
	vector<int> newDistances(V, INT_MAX); // Расстояния к новому городу
	vector<int> newDistances2(V, INT_MAX); // Расстояния от нового города

	cout << "Расстаяния от нового города: \n";
	for (int i = 0; i < V; ++i) {
		cout << " к городу " << i + 1 << " : ";
		cin >> newDistances[i];
	}
	cout << "Расстояние до нового города:\n";
	for (int i = 0; i < V; ++i) {
		cout << " от города " << i + 1 << " : ";
		cin >> newDistances2[i];
	}

	// Добавляем расстояния от нового города в существующие строки
	for (int i = 0; i < V; ++i) {
		cities[i].push_back(newDistances2[i]);
	}

	// Добавляем новый город в матрицу
	newDistances.push_back(INT_MAX); // Сам к себе расстояние бесконечное
	cities.push_back(newDistances); // Добавляем новый город в матрицу
	V++; // Увеличиваем количество городов
}

// Функция deleteCity
// -------------------
// Цель:
// Удаляет город из матрицы расстояний.
//
// Алгоритм:
// 1. Удаляются элементы в каждой строке матрицы, соответствующие индексу удаляемого города.
// 2. Полностью удаляется строка, связанная с удаляемым городом.
// 3. Уменьшается общее количество городов (V).
//
// Переменные:
// - cityIndex: Индекс удаляемого города.
// - V: Количество городов в матрице.
void deleteCity(int cityIndex) {
	for (int i = 0; i < V; ++i) {
		cities[i].erase(cities[i].begin() + cityIndex); // Удаляем соответствующие столбцы
	}
	cities.erase(cities.begin() + cityIndex); // Удаляем строку, связанную с городом
	V--; // Уменьшаем общее количество городов
}

// Вывод матрицы на экран
void printMatrix(vector<vector<int>> matrix) {
	for (auto i : matrix) {
		for (auto el : i) {
			if (el != INT_MAX)
				cout << el << "\t";
			else
				cout << "INF" << "\t";
		}
		cout << endl;
	}
}

// Главная функция программы
int main() {
	setlocale(LC_ALL, "ru"); // Устанавливаем русскую локализацию
	srand(time(0)); // Инициализируем генератор случайных чисел
	int input = 0;
	cout << "Стартовая матрица городов: \n";
	printMatrix(cities);
	cout << endl;

	while (true) {
		// Главное меню программы
		cout << "\n1 - редактировать матрицу городов\n";
		cout << "2 - запустить генетический алгоритм\n";
		cout << "3 - выход\n";
		cin >> input;
		switch (input) {
		case 1: {
			// Меню редактирования матрицы
			cout << "\nМеню редактирования матрицы\n";
			cout << "1 - добавить город\n";
			cout << "2 - убрать город\n";
			cin >> input;
			switch (input) {
			case 1: {
				addCity();
				cout << "\nНовая матрица городов: \n";
				printMatrix(cities);
				break;
			}
			case 2: {
				cout << "\nНомер удаляемого города (от 0 до " << V - 1 << "): \n";
				cin >> input;
				deleteCity(input);
				cout << "\nНовая матрица городов: \n";
				printMatrix(cities);
				break;
			}
			}
			break;
		}
		case 2: {
			// Настройки генетического алгоритма
			cout << "Размер популяции: ";
			cin >> populationSize;
			if (populationSize < 3)
			{
				cout << "Размер популяции должен быть больше 2\n";
				break;
			}
			cout << "Вероятность мутации (в процентах): ";
			cin >> mutationChance;
			cout << "Количество скрещиваний в одной популяции: ";
			cin >> child;
			cout << "Количество эволюций: ";
			cin >> evoCount;

			int gen = 1; // Текущая эволюция
			int mutated_counter = 0; // Счетчик мутаций

			vector<struct Path> population; // Вектор популяции
			struct Path temp;

			// Создаем начальную популяцию
			for (int i = 0; i < populationSize; i++) {
				temp.path = create_path();
				temp.lenght = calculate_lenght(temp.path);
				population.push_back(temp);
			}

			cout << "\nСтартовая популяция: " << endl;
			cout << "Путь популяции       Длина пути\n";
			for (int i = 0; i < populationSize; i++)
				cout << population[i].path << "\t\t\t"
				<< population[i].lenght << endl;
			cout << "\n";

			// Сортируем начальную популяцию
			sort(population.begin(), population.end(), lessthan);

			// Основной цикл эволюции
			while (gen <= evoCount) {
				mutated_counter = 0;
				cout << endl << "Лучший путь: " << population[0].path;
				cout << " его длина: " << population[0].lenght << "\n\n";

				vector<struct Path> new_population;

				// Создаем потомков
				for (int i = 0; i < child; i++) {
					struct Path p1 = population[i];

					while (true) {
						int dad_id = rand_num(0, populationSize - 1);
						int mom_id = rand_num(0, populationSize - 1);
						if (dad_id != mom_id) {
							Path new_marshrut = childGene(population[dad_id], population[mom_id]);
							new_population.push_back(new_marshrut);
							new_marshrut = childGene(population[mom_id], population[dad_id]);
							new_population.push_back(new_marshrut);
							break;
						}
					}
				}

				// Мутация
				for (int i = 0; i < populationSize - 1; i++) {
					struct Path p1 = population[i];

					if (rand_num(0, 100) > mutationChance) {
						mutated_counter++;
						while (true) {
							string new_path = mutatedGene(p1.path);
							struct Path new_marshrut;
							new_marshrut.path = new_path;
							new_marshrut.lenght = calculate_lenght(new_marshrut.path);

							if (new_marshrut.lenght <= population[i].lenght) {
								new_population.push_back(new_marshrut);
								break;
							}
							else {
								new_marshrut.lenght = INT_MAX;
								new_population.push_back(new_marshrut);
								break;
							}
						}
					}
				}

				// Обновление популяции
				for (int i = 0; i < child + mutated_counter; i++) {
					population.push_back(new_population[i]);
				}
				sort(population.begin(), population.end(), lessthan);

				for (int i = 0; i < child + mutated_counter; i++) {
					population.pop_back();
				}

				// Вывод текущего поколения
				cout << "Поколение " << gen << " \n";
				cout << "Путь популяции \t|| Длина пути\n";

				for (int i = 0; i < populationSize; i++)
					cout << population[i].path << "\t\t\t"
					<< population[i].lenght << endl;
				gen++;
			}

			// Итоговый оптимальный маршрут
			cout << "\n\nНаиболее оптимальный маршрут: ";
			cout << population[0].path;
			cout << "\n\n";
			return 0;
		}
		case 3:
			return 0;
		}
	}
}
