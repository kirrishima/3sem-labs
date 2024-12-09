#include "pch.h"
#include "framework.h"
#include <Windows.h>

extern "C" void __stdcall __PrintNumber(int n)
{
	std::cout << "number: " << n << std::endl;
}

extern "C" void __stdcall __PrintBool(bool b)
{
	std::cout << (b ? "True" : "False") << std::endl;
}

extern "C" void __stdcall __Print(char* arr)
{
	SetConsoleOutputCP(1251);

	int n = strlen(arr);
	for (int i = 0; i < n; ++i) {
		if (arr[i] == '\\' && i + 1 < n) {
			char nextChar = arr[++i];
			switch (nextChar) {
			case 'n': std::cout << '\n'; break;
			case 't': std::cout << '\t'; break;
			case '\\': std::cout << '\\'; break;
			case '"': std::cout << '"'; break;
			case '\'': std::cout << '\''; break;
			default:
				std::cout << '\\' << nextChar;
				break;
			}
		}
		else {
			std::cout << ((const char*)arr)[i];
		}
	}
	std::cout << std::endl;
}

//extern "C" void __stdcall __PrintArray(void* arr, int n, int type)
//{
//	SetConsoleOutputCP(1251);
//	void* ptr;
//	if (type == sizeof(char))
//	{
//		std::cout << "char[]: ";
//
//		for (int i = 0; i < n; ++i) {
//			if (((const char*)arr)[i] == '\\' && i + 1 < n) {
//				char nextChar = ((const char*)arr)[++i];
//				switch (nextChar) {
//				case 'n': std::cout << '\n'; break;
//				case 't': std::cout << '\t'; break;
//				case '\\': std::cout << '\\'; break;
//				case '"': std::cout << '"'; break;
//				case '\'': std::cout << '\''; break;
//				default:
//					std::cout << '\\' << nextChar;
//					break;
//				}
//			}
//			else {
//				std::cout << ((const char*)arr)[i];
//			}
//		}
//	}
//	if (type == sizeof(short))
//	{
//		std::cout << "short[]: ";
//		for (int i = 0; i < n; i++)
//		{
//			std::cout << *(reinterpret_cast<const short*>(arr) + i) << " ";
//		}
//	}
//
//	if (type == sizeof(int))
//	{
//		std::cout << "int[]: ";
//		for (int i = 0; i < n; i++)
//		{
//			std::cout << *(reinterpret_cast<const int*>(arr) + i) << " ";
//		}
//	}
//
//	std::cout << std::endl;
//}

extern "C" int __stdcall __StrCmp(const char* _str1, const char* _str2)
{
	int result = strcmp(_str1, _str2);
	return (result < 0) ? -1 : (result > 0) ? 1 : 0;
}
