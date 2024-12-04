#pragma once
#include <string>
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <format>
#include <stack>

using namespace std;

namespace CD
{
	struct CodeGeneration {
		std::string tab = "    ";

		IT::ID_Table ID_TABLE;
		LT::LexTable LEX_TABLE;
		std::ofstream OUT_ASM_FILE;

		static std::string __getIDnameInDataSegment(const IT::Entry& entry);

		CodeGeneration(const IT::ID_Table& ID_TABLE, const LT::LexTable& LEX_TABLE, const std::wstring& OUT_FILEPATH)
			: ID_TABLE(ID_TABLE), LEX_TABLE(LEX_TABLE), ifElseGeneration(*this)
		{
			this->OUT_ASM_FILE.open(OUT_FILEPATH);
		}

		std::vector<std::string> __generate_math_expressions(const std::string& expr);
		void __s_const();
		void __s_data();

		std::string lexem_to_source(LT::Entry& entry)
		{
			switch (entry.lexema[0]) {
			case 'p':
				return "print";

			case 'l':
				if (ID_TABLE.table[entry.idxTI].iddatatype == IT::IDDATATYPE::INT)
				{
					return to_string(ID_TABLE.table[entry.idxTI].value.vint);
				}
				if (ID_TABLE.table[entry.idxTI].iddatatype == IT::IDDATATYPE::STR)
				{
					return ID_TABLE.table[entry.idxTI].value.vstr->str;
				}
				throw "������� ������������ ���� ������";

			case 'i':
				return ID_TABLE.table[entry.idxTI].id;

			case 'c':
				return entry.c;

			case 'v':
			{
				std::string result = " ";
				result[0] = entry.v;
				return result;
			}

			case LEX_INTEGER:
				if (ID_TABLE.table[entry.idxTI].iddatatype == IT::IDDATATYPE::INT)
				{
					return "int";
				}
				throw "������������� ������������ ���� ������";
			default: {
				std::string result = " ";
				result[0] = entry.lexema[0];
				return result;
			}
			}
		}

		std::vector<std::string> parse_expression(int& index_in_lex_table);
		std::string __get_string_value(const int lex_id);
		std::string __lexemVectorIDStoString(const vector<int>& ids);

		void gen(const std::wstring& OUT_FILEPATH, bool);

		/// <summary>
		/// ��������� �������� ����������
		/// </summary>
		struct IfElseGeneration {
			CodeGeneration& parent; // ������ �� ������� ���������

			IfElseGeneration(CodeGeneration& p) : parent(p) {}

			int IFLabelsCount = 0; // ������� �����, ������������ ��� ����������� ���������� if-else �����
			int currentElseLabel = 0; // ������� ����� else, ��� �������� ������� ����� else

			int nestingLevel = 0; // ������� �����������, ������������ ��� ��������

			std::stack<std::string> if_stack;

			void GenerateCondition(
				const std::vector<std::string>& operands, // ��� �������� - ����� � ������ 
				const std::string& comparison, // �������� ��������� (>, <, ==, !=, >=, <=)
				const std::string& trueLabel, // ��� ����� ���� ������� �����������
				const std::string& falseLabel, // ���� �� �����������
				std::vector<std::string>& instructions, bool isStringCmp = false
			);

			void CompareInts(std::vector<std::string>& instructions, const vector<string>& operands);
			void CompareStrings(std::vector<std::string>& instructions, const string& str1Name, const string& str2Name);

			void StartIf(const vector<vector<int>>& operands, //2 ��������: ����� � ������
				const string&,  // �������� (>, <, ==, !=, >=, <=)
				std::vector<std::string>& // ������ � ������������, � ������� ����� �������� ��������������� �����
			);
			void StartElse(std::vector<std::string>& instructions);
			void EndIfOrElse(std::vector<std::string>& instructions);
			void EndExpression(std::vector<std::string>& instructions);

			std::vector<std::string> generateIfStatement(int& i);

			string GenerateLabel(const string& prefix, int n) {
				return prefix + "_" + to_string(n);
			}
		} ifElseGeneration;
	};
}