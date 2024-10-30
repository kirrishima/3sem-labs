#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <Windows.h>

struct Node
{
	char ch;
	int freq;
	int index;
	Node* left;
	Node* right;

	Node(char c, int f, int i) : ch(c), freq(f), index(i), left(nullptr), right(nullptr) {}
};

// Функция сравнения для приоритетной очереди (минимальная частота на первом месте)
struct Compare
{
	bool operator()(Node* left, Node* right)
	{
		if (left->freq == right->freq)
		{
			return left->index < right->index;
		}
		return left->freq > right->freq;
	}
};

// Функция для печати кодов Хаффмана
void printCodes(struct Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode)
{
	if (root == nullptr)
		return;

	// Если найден листовой узел
	if (!root->left && !root->right)
	{
		huffmanCode[root->ch] = str;
	}

	printCodes(root->left, str + "0", huffmanCode);
	printCodes(root->right, str + "1", huffmanCode);
}

// Функция для построения дерева Хаффмана и кодирования символов
void buildHuffmanTree(const std::string& text)
{
	// Подсчет частоты каждого символа
	std::unordered_map<char, int> freq;
	// Индекс первого вхождения в строку
	std::unordered_map<char, int> firstIndex;

	int x = 0;
	for (char ch : text)
	{
		freq[ch]++;
		if (firstIndex.find(ch) == firstIndex.end())
		{
			firstIndex[ch] = x;
		}
		x++;
	}

	// Создание приоритетной очереди
	std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

	// Создание узла для каждого символа и добавление в очередь
	for (auto pair : freq)
	{
		pq.push(new Node(pair.first, pair.second, firstIndex[pair.first]));
	}

	// Построение дерева Хаффмана
	while (pq.size() != 1)
	{
		// Два узла с наименьшей частотой
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top(); pq.pop();

		// Создание нового внутреннего узла с суммой частот
		int sum = left->freq + right->freq;
		Node* node = new Node('\0', sum, max(left->index, right->index));
		node->left = left;
		node->right = right;

		pq.push(node);
	}

	// Корневой узел
	Node* root = pq.top();

	// Таблица для хранения кодов символов
	std::unordered_map<char, std::string> huffmanCode;
	printCodes(root, "", huffmanCode);

	// Вывод таблицы частот
	std::cout << "Частота символов:\n";
	for (auto pair : freq)
	{
		std::cout << pair.first << ": " << pair.second << '\n';
	}

	// Вывод кодов Хаффмана
	std::cout << "\nКоды Хаффмана:\n";
	for (auto pair : huffmanCode)
	{
		std::cout << '\'' << pair.first << '\'' << ": " << pair.second << '\n';
	}

	// Кодирование текста
	std::string encodedString;
	for (char ch : text)
	{
		encodedString += huffmanCode[ch];
	}

	// Вывод закодированной строки
	std::cout << "\nЗакодированная последовательность:\n" << encodedString << '\n';
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	std::string text;
	std::cout << "Введите текст для кодирования: ";
	std::getline(std::cin, text);

	std::cout << text << std::endl;
	buildHuffmanTree(text);

	return 0;
}
