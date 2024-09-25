#include "FST.h"
#include <iostream>
#include <TCHAR.H>
#include <clocale>

using namespace std;

using FST::NODE;
using FST::RELATION;

void ExecuteAndProcessFST(FST::FST& fst);
FST::FST* CreateFST(const char* str);
void ParseString(const char* str);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");

	// По аналогии рис. 2 разработайте код для проверки семи цепочек символов, 
	// подготовленных в рамках лабораторной работы 13

	ParseString("o w; c");					// open□write;□close 
	ParseString("o   w;r;   c");			// open□□□write;read□□□close 
	ParseString("o w;r;s;    c");			// open□write;read;seek;□□□□close 
	ParseString("o     w;s; c");			// open□□□□□write;seek;□close 
	ParseString("o    w;r;s;w;r;s;    c");  // open□□□□write;read;seek;write;read;seek;□□□□close 
	ParseString("o w;s;r;r;s; c");			// open□write;seek;read;read;seek;□close 
	ParseString("o    w;r;s;s;s;      c");  // open□□□□write;read;seek;seek;seek;□□□□□□close

	//Предложите цепочку, при которой разбор проходит все символы входной 
	//цепочки, но при этом, цепочка не распознается.
	cout << endl;
	auto fst = CreateFST("o    w;r;s;s;s;      ");
	ExecuteAndProcessFST(*fst);
	cout << "номер символа, на котором завершился разбор (начинается с 1): " << fst->position + 1 << ", " << "длина строки: " << strlen(fst->string) << endl;
	delete fst;

	//Предложите цепочку, при которой разбор завершается, не перебрав все
	//символы входной цепочки
	cout << endl;
	fst = CreateFST("o s;;s;w; c");
	ExecuteAndProcessFST(*fst);
	cout << "номер символа, на котором завершился разбор (начинается с 1): " << fst->position + 1 << ", " << "длина строки: " << strlen(fst->string) << endl;
	delete fst;

	return 0;
}

void ParseString(const char* str)
{
	FST::FST* fst = CreateFST(str);
	ExecuteAndProcessFST(*fst);
	delete fst;
}

void ExecuteAndProcessFST(FST::FST& fst)
{
	if (FST::execute(fst))
	{
		cout << "Цепочка \"" << fst.string << "\" распознана" << endl;
	}
	else
	{
		cout << "Цепочка \"" << fst.string << "\" не распознана" << endl;
	}
}

FST::FST* CreateFST(const char* str)
{
	FST::FST* fst = new FST::FST(
		str,
		7,
		NODE(1, RELATION('o', 1)),					 /*0*/

		NODE(2, RELATION(' ', 1), RELATION(' ', 2)), /*1*/

		NODE(3,										 /*2*/
			RELATION('w', 3),
			RELATION('r', 3),
			RELATION('s', 3)),

		NODE(1,										 /*3*/
			RELATION(';', 4)),

		NODE(5,										 /*4*/
			RELATION(' ', 5),
			RELATION('w', 3),
			RELATION('r', 3),
			RELATION('s', 3)
		),

		NODE(2, RELATION(' ', 5), RELATION('c', 6)), /*5*/

		NODE()										 /*6*/
	);

	return fst;
}