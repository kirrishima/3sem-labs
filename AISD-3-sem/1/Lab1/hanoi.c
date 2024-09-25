#include "stdio.h"
#include "locale.h"
#include "stdbool.h"

void hanoi(int n, int i, int k);

int main()
{
	setlocale(LC_ALL, "Russian");
	int n, i, k;
	printf("Введите количество дисков n, начальный номер стержня i, целевой стержень k, через пробел: ");
	if (scanf_s("%d %d %d", &n, &i, &k) != 3)
	{
		printf("Неправильный ввод. Ожидалось 3 числа\n");
		system("pause");
		return -1;
	}

	bool flag = false;

	if (n < 1)
	{
		printf("n должно быть больше 0\n");
		flag = true;
	}

	if (k != 2 && k != 3 && k != 1)
	{
		printf("k должно быть от 1 до 3\n");
		flag = true;
	}

	if (i != 2 && i != 3 && i != 1)
	{
		printf("i должно быть от 1 до 3\n");
		flag = true;
	}

	if (k == i || flag)
	{
		return 0;
	}

	printf("\n");
	hanoi(n, i, k);
	printf("\n");

	system("pause");
	return 0;
}

void hanoi(int n, int i, int k)
{
	if (n == 1)
	{
		printf("Переместить диск %d с %d на %d стержень\n", n, i, k);
		return;
	}
	int tmp = 6 - i - k;
	hanoi(n - 1, i, tmp);
	printf("Переместить диск %d с %d на %d стержень\n", n, i, k);
	hanoi(n - 1, tmp, k);
}