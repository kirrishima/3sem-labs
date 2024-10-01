#include "stdafx.h"
#include "IT.h"
#include "Error.h"


namespace IT
{
	IdTable Create(int size)
	{
		IdTable* tabl = new IdTable;
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(116);
		}
		tabl->maxsize = size;
		tabl->size = 0;
		tabl->table = new Entry[size];
		return *tabl;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size + 1 > idtable.maxsize)
		{
			throw ERROR_THROW(117);
		}
		idtable.table[idtable.size] = entry;
		idtable.size += 1;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int search(IdTable& idtable, IT::Entry entry)
	{
		for (int i = 0; i <= idtable.size; i++)
		{
			if (strcmp(entry.id, idtable.table[i].id) == NULL && entry.scope == idtable.table[i].scope)
			{
				return i;
			}
			else if (strcmp(entry.id, idtable.table[i].id) == NULL && idtable.table[i].idtype == IT::F)
			{
				return i;
			}
			else
				continue;
		}
		return -1;
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
			if (strcmp(idtable.table[i].id, id) == 0)
				return i;

		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		idtable.table = nullptr;
	}
}