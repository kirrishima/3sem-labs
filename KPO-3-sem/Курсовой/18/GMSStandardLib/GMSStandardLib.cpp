#include "pch.h"
#include "framework.h"
#include <Windows.h>

extern "C" void __stdcall __PrintNumber(int n)
{
	std::cout << "number: " << n << std::endl;
}

extern "C" void __stdcall __PrintBool(bool b)
{
	std::cout << (b ? "True" : "False");
}

extern "C" void __stdcall __PrintArray(void* arr, int n, int type)
{
	SetConsoleOutputCP(1251);
	void* ptr;
	if (type == sizeof(char))
	{
		std::cout << "char[]: ";
		for (int i = 0; i < n; i++)
		{
			std::cout << *(reinterpret_cast<const char*>(arr) + i);
		}
	}
	if (type == sizeof(short))
	{
		std::cout << "short[]: ";
		for (int i = 0; i < n; i++)
		{
			std::cout << *(reinterpret_cast<const short*>(arr) + i) << " ";
		}
	}

	if (type == sizeof(int))
	{
		std::cout << "int[]: ";
		for (int i = 0; i < n; i++)
		{
			std::cout << *(reinterpret_cast<const int*>(arr) + i) << " ";
		}
	}

	std::cout << std::endl;
}

extern "C" int __stdcall __StrCmp(const char* _str1, const char* _str2)
{
	int result = strcmp(_str1, _str2);
	return (result < 0) ? -1 : (result > 0) ? 1 : 0;
}
