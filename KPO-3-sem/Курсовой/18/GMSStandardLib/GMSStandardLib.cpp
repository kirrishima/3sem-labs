// GMSStandardLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

// TODO: This is an example of a library function
extern "C" void __stdcall __PrintNumber(int n)
{
	std::cout << "number: " << n << std::endl;
}


extern "C" void __stdcall __PrintArray(void* arr, int n, int type)
{
	void* ptr;
	if (type == 1)
	{
		std::cout << "char: ";
		for (int i = 0; i < n; i++)
		{
			std::cout << *(reinterpret_cast<const char*>(arr) + i);
		}
	}
	if (type == 2)
	{
		std::cout << "short: ";
		for (int i = 0; i < n; i++)
		{
			std::cout << *(reinterpret_cast<const short*>(arr) + i) << " ";
		}
	}

	if (type == 4)
	{
		std::cout << "int: ";
		for (int i = 0; i < n; i++)
		{
			std::cout << *(reinterpret_cast<const int*>(arr) + i) << " ";
		}
	}

	std::cout << std::endl;
}

