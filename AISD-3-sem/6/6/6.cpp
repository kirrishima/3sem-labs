#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <Windows.h>

struct Node {
	int number;
	std::string symbol = "";
	Node* left, * right;
	Node(int num, const std::string& sym, Node* l = nullptr, Node* r = nullptr)
		: number(num), symbol(sym), left(l), right(r) {}
	Node() {}
};


Node* HuffmanAlgorithm(std::list<Node*>& nodes) {
	while (nodes.size() != 1) {
		nodes.sort([](Node* firstNode, Node* secondNode) -> bool { return firstNode->number < secondNode->number; });

		Node* left = nodes.front();
		nodes.pop_front();
		Node* right = nodes.front();
		nodes.pop_front();

		Node* parent = new Node;
		parent->left = left;
		parent->right = right;
		parent->symbol += left->symbol + right->symbol;
		parent->number = left->number + right->number;

		nodes.push_back(parent);
	}

	return nodes.front();
}

void BuildTable(Node* root, std::vector<bool>& code, std::map<char, std::vector<bool>>& matchingTable) {
	if (root->left != nullptr) {
		code.push_back(0);
		BuildTable(root->left, code, matchingTable);
	}

	if (root->right != nullptr) {
		code.push_back(1);
		BuildTable(root->right, code, matchingTable);
	}

	if (root->left == nullptr && root->right == nullptr) {
		matchingTable[root->symbol[0]] = code;
	}

	if (!code.empty()) {
		code.pop_back();
	}
}


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::map<char, int> counter;
	std::map<char, std::vector<bool>> matchingTable;
	std::list<Node*> nodes;
	std::vector<bool> code;
	std::string text;

	getline(std::cin, text);
	for (int i = 0; i < text.size(); i++) {
		counter[text[i]]++;
	}

	std::set<int, std::greater<int>> uniqueNumbers;
	std::list<char> nnn;

	for (const auto& pair : counter) {
		uniqueNumbers.insert(pair.second);
	}

	std::cout << "\nЧастота:\n";
	for (auto elem : uniqueNumbers)
	{
		auto it = counter.rbegin();
		for (int j = counter.size() - 1; j >= 0; j--)
		{
			auto p = *it++;
			if (p.second == elem)
			{
				std::cout << '\'' << p.first << '\'' << " -> " << p.second << std::endl;
				nnn.push_back(p.first);
			}
		}
	}

	for (auto p : counter) {
		nodes.push_back(new Node{ p.second, *(new std::string(1, p.first)), nullptr, nullptr });
	}

	std::cout << std::endl;
	Node* root = HuffmanAlgorithm(nodes);
	BuildTable(root, code, matchingTable);

	std::cout << "\nКоды символов:\n";
	//for (const auto& itm : matchingTable) {
	//	std::cout << '\'' << itm.first << '\'' << " = ";
	//	for (int i = 0; i < itm.second.size(); i++) {
	//		std::cout << itm.second[i];
	//	}
	//	std::cout << std::endl;
	//}

	for (auto elem : nnn)
	{
		std::cout << '\'' << elem << '\'' << ": ";
		for (auto b : matchingTable[elem])
		{
			std::cout << b;
		}
		std::cout << '\n';
	}

	std::cout << "\nКод строки: ";
	for (int i = 0; i < text.size(); i++) {
		const std::vector<bool>& temp = matchingTable.at(text[i]);
		int sizeTemp = temp.size();
		for (int j = 0; j < sizeTemp; j++) {
			std::cout << temp[j];
		}
	}
	std::cout << std::endl;


	return 0;
}