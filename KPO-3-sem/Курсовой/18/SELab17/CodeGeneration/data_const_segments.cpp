#include "stdafx.h"
#include "cd.h"

using namespace std;

void CD::CodeGeneration::__s_const()
{
	OUT_ASM_FILE << ".const\n";
	for (int i = 0; i < ID_TABLE.size; i++)
	{
		if (ID_TABLE.table[i].idtype == IT::L)
		{
			if (ID_TABLE.table[i].iddatatype == IT::IDDATATYPE::INT)
			{
				OUT_ASM_FILE << tab << get_id_name_in_data_segment(ID_TABLE.table[i]) << " SDWORD " << ID_TABLE.table[i].value.vint << '\n';
			}
			if (ID_TABLE.table[i].iddatatype == IT::IDDATATYPE::STR)
			{
				OUT_ASM_FILE << tab << get_id_name_in_data_segment(ID_TABLE.table[i]) << " db \"" << ID_TABLE.table[i].value.vstr->str << "\", 0\n";
			}
		}
	}
}

void CD::CodeGeneration::__s_data()
{
	OUT_ASM_FILE << ".data\n" << tab << reservedBoolName << ' ' << "byte" << " ? \n";

	for (int i = 0; i < ID_TABLE.size; i++)
	{
		auto entry = &ID_TABLE.table[i];
		if (entry->idtype == IT::IDTYPE::V)
		{
			if (entry->iddatatype == IT::INT)
			{
				OUT_ASM_FILE << tab << get_id_name_in_data_segment(*entry)
					<< " sdword "
					<< "0\n";
			}
			//if (entry->iddatatype == IT::STR)
			//{
			//	OUT_ASM_FILE << tab << get_id_name_in_data_segment(*entry);
			//	//if (entry->value.vstr->len > 0)
			//	//{
			//	//	OUT_ASM_FILE << " db ";

			//	//	OUT_ASM_FILE << '"' << entry->value.vstr->str << "\" , 0\n";
			//	//}
			////else
			////{
			//	OUT_ASM_FILE << " dword ?\n";
			//	/*}*/
			//}
		}
	}
}

std::string CD::CodeGeneration::get_id_name_in_data_segment(const IT::Entry& entry)
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