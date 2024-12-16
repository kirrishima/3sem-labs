#include "stdafx.h"
#include <cstring>
#include "FST.h"
#include <regex>

namespace FST
{
	NODE::NODE(short numRelations, RELATION firstRelation, ...)
	{
		n_relation = numRelations;
		RELATION* relationsArray = &firstRelation;
		relations = new RELATION[numRelations];

		for (short i = 0; i < numRelations; i++) {
			relations[i] = relationsArray[i];
		}
	}

	FST::FST(const char* inputString, short numStates, NODE firstNode, ...)
	{
		string = inputString;
		nstates = numStates;
		nodes = new NODE[numStates];
		NODE* nodesArray = &firstNode;

		for (int i = 0; i < numStates; i++) {
			nodes[i] = nodesArray[i];
		}

		rstates = new short[nstates];

		std::memset(rstates, 0xFF, sizeof(short) * nstates);

		rstates[0] = 0;  // начинаем разбор с начального состояния (0)
	}


	// Функция execute для выполнения распознавания цепочки
	bool execute(FST& fst)
	{
		short* tmpStates = new short[fst.nstates];
		// сохраняем состояния для возможности переиспользования (повторного разбора) объекта FST 
		std::memcpy(tmpStates, fst.rstates, fst.nstates * sizeof(short));

		short* rstates = new short[fst.nstates];
		std::memset(rstates, -1, fst.nstates * sizeof(short));  // инициализируем массив значением -1

		short lstring = std::strlen(fst.string);  // длина строки

		bool rc = true;  // результат распознавания

		for (fst.position = 0; fst.position < lstring && rc; fst.position++)
		{
			rc = false;
			std::memcpy(rstates, fst.rstates, fst.nstates * sizeof(short));  // копируем текущие состояния
			std::memset(fst.rstates, -1, fst.nstates * sizeof(short));  // обнуляем массив состояний

			for (int i = 0; i < fst.nstates; i++)
			{
				if (rstates[i] == fst.position)  // если текущее состояние возможно
				{
					for (int j = 0; j < fst.nodes[i].n_relation; j++)  // проверяем все ребра
					{
						if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])  // если символ совпадает
						{
							fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;  // переходим в новое состояние
							rc = true;  // продолжаем распознавание
						}
					}
				}
			}
		}

		delete[] rstates;
		auto result = rc ? fst.rstates[fst.nstates - 1] == lstring : rc;

		std::memcpy(fst.rstates, tmpStates, fst.nstates * sizeof(short));
		return result;  // проверяем, достигли ли конечного состояния
	}

	bool execute(const char* regex, const char* str)
	{
		std::regex pattern(regex);
		return std::regex_match(str, pattern);
	}
}
