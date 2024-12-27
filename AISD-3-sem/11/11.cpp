#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <Windows.h>

bool randomChoiceStrategy(const std::vector<int>& boxes, int prisonerNumber) {
	std::vector<int> openedBoxes = boxes;
	std::shuffle(openedBoxes.begin(), openedBoxes.end(), std::mt19937(std::random_device()()));
	for (int i = 0; i < 50; ++i) {
		if (openedBoxes[i] == prisonerNumber) {
			return true;
		}
	}
	return false;
}

bool loopStrategy(const std::vector<int>& boxes, int prisonerNumber) {
	int nextBox = prisonerNumber - 1;
	for (int i = 0; i < 50; ++i) {
		if (boxes[nextBox] == prisonerNumber) {
			return true;
		}
		nextBox = boxes[nextBox] - 1;
	}
	return false;
}

int simulateGame(int rounds, bool (*strategy)(const std::vector<int>&, int)) {
	int successfulOutcomes = 0;

	for (int round = 0; round < rounds; ++round) {
		std::vector<int> boxes(100);
		for (int i = 0; i < 100; ++i) {
			boxes[i] = i + 1;
		}
		std::shuffle(boxes.begin(), boxes.end(), std::mt19937(std::random_device()()));

		bool allPrisonersSucceeded = true;
		for (int prisonerNumber = 1; prisonerNumber <= 100; ++prisonerNumber) {
			if (!strategy(boxes, prisonerNumber)) {
				allPrisonersSucceeded = false;
				break;
			}
		}

		if (allPrisonersSucceeded) {
			++successfulOutcomes;
		}
	}

	return successfulOutcomes;
}

int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	std::cout << "������ �100 ������������\n";

	int rounds;
	std::cout << "������� ���������� ������� ��� ���������: ";
	std::cin >> rounds;

	int randomSuccess = simulateGame(rounds, randomChoiceStrategy);
	int loopSuccess = simulateGame(rounds, loopStrategy);

	std::cout << "\n���������� ����� " << rounds << " �������:\n";
	std::cout << "��������� ���������: " << randomSuccess << " �������� �������\n";
	std::cout << "��������� ������: " << loopSuccess << " �������� �������\n";

	return 0;
}
