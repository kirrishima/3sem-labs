#include "stdafx.h"
#include "cd.h"

using namespace std;

void CD::CodeGeneration::__s_const()
{
	OUT_ASM_FILE << ".const\n";
	for (size_t i = 0; i < ID_TABLE.size; i++)
	{
		if (ID_TABLE.table[i].idtype == IT::L)
		{
			if (ID_TABLE.table[i].iddatatype == IT::INT)
			{
				OUT_ASM_FILE << '\t' << __getIDnameInDataSegment(ID_TABLE.table[i]) << " SDWORD " << ID_TABLE.table[i].value.vint << '\n';
			}
		}
	}
}

void CD::CodeGeneration::__s_data()
{
	OUT_ASM_FILE << ".data\n";

	for (size_t i = 0; i < ID_TABLE.size; i++)
	{
		auto entry = &ID_TABLE.table[i];
		if (entry->idtype == IT::IDTYPE::V)
		{
			if (entry->iddatatype == IT::INT)
			{
				OUT_ASM_FILE << "\t" << __getIDnameInDataSegment(*entry)
					<< " SDWORD "
					<< "0\n";
			}
		}
	}
}

std::string CD::CodeGeneration::__getIDnameInDataSegment(const IT::Entry& entry)
{
	static unordered_map <int, std::string> var;

	if (var.find((unsigned int)entry.id) != var.end())
	{
		return var[(unsigned int)entry.id];
	}

	ostringstream ss;

	if (entry.idtype == IT::IDTYPE::L)
	{
		ss << "__" << entry.id;
	}
	else if (entry.idtype == IT::IDTYPE::V)
	{
		ss << "_";

		if (entry.scope != NULL)
		{
			ss << entry.scope;
		}

		ss << "__" << entry.id;
	}
	else
	{
		throw "Неизвестный ID при генерации имени в сегменте .data";
	}

	std::string result = ss.str();
	var[(int)entry.id] = result;

	return result;
}