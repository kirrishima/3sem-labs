#pragma once
#include <string>
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <format>
#include <stack>

using namespace std;

std::string trim(const std::string& str);
bool isMASMLabel(const std::string& line);
std::string removeComment(const std::string& str);
bool isValidIdentifier(const std::string& identifier);
bool isFunctionStart(const std::string& line);
bool isFunctionEnd(const std::string& line);

std::string operator*(const std::string& str, int times);

namespace CD
{
	bool is_assignment(const std::string& expr);

	struct UserDefinedFunctions
	{
		std::string name;
		vector<IT::IDDATATYPE> params;
		vector<string> code;

		void push_code(string code_instruction) {
			code.push_back(code_instruction);
		}

		void push_code(const char* code_instruction) {
			code.push_back(string(code_instruction));
		}

		void push_params(const IT::IDDATATYPE type) {
			params.push_back(type);
		}
	};

	struct CodeGeneration {
		std::string tab = "    ";
		const std::string reservedBoolName = "_@bool";
		int trueLabelsCount = 0;

		IT::ID_Table ID_TABLE;
		LT::LexTable LEX_TABLE;
		std::ofstream OUT_ASM_FILE;

		vector<UserDefinedFunctions> user_functions;

		CodeGeneration(const IT::ID_Table& ID_TABLE, const LT::LexTable& LEX_TABLE, const std::wstring& OUT_FILEPATH)
			: ID_TABLE(ID_TABLE), LEX_TABLE(LEX_TABLE), ifElseGen(*this)
		{
			this->OUT_ASM_FILE.open(OUT_FILEPATH);
		}

		static std::string get_id_name_in_data_segment(const IT::Entry& entry);
		vector<string> parseExpression(const std::vector<int>& expression);
		std::vector<std::string> generate_math_expressions(const std::vector<int>& expr);
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
				throw "Литерал неизвестного типа данных";

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
				throw "Идентификатор неизвестного типа данных";
			default: {
				std::string result = " ";
				result[0] = entry.lexema[0];
				return result;
			}
			}
		}

		std::vector<std::string> parse_function_call(UserDefinedFunctions& function, int params_start_index, int params_end_index);
		void parse_function(int start_index, int end_index);
		void parse_function_body(UserDefinedFunctions& function, int start_index, int end_index);
		std::vector<std::string> parse_lexem(int& index_in_lex_table);
		vector<string> parse_lexem_equal__(int& index_in_lex_table);

		struct ParseExpressionReturnParms
		{
			bool isSingleVariable = true;
			bool isStringPTR = false;

			bool isResultComputed = false;
			bool isResultInDefaultBool = false;
			bool isResultInEAX = false;
			bool isResultInEBX = false;
			bool isResultInSTACK = false;

			bool isStringCompare = false;
			bool isIntCompare = false;
			bool isCompare = false;

			bool isMath = false;
			bool isINT = false;
			bool isSTR = false;
			bool isFunctionCall = false;

			IT::IDDATATYPE returnDataType;
			std::string stringRepresentation;
			std::string resultStorage;
		};

		/// <summary>
		/// выражение присваивания, параметры функции print, условия  
		/// </summary>
		/// <param name="ids"></param>
		/// <param name="instructions"></param>
		/// <returns></returns>
		ParseExpressionReturnParms parse_expression(vector<int> ids, std::vector<std::string>& instructions);
		vector<string> parse_print_lexem__(int& i);

		/// <summary>
		/// Возвращает строку, представляющую собой текущее значение строковой переменной в виде имени литерала
		/// </summary>
		/// <param name="lex_id"></param>
		/// <returns></returns>
		std::string get_string_value(const int lex_id);
		//// <summary>
		/// Преобразует входной вектор индексов лексем в строку, представляющую выражение с именами на асм.
		/// Используется для генерации польской натации по выражению
		/// </summary>
		/// <param name="ids">индексы лесем</param>
		/// <returns></returns>
		std::string lexems_vector_to_string(const vector<int>& ids);

		void gen(const std::wstring& OUT_FILEPATH);

		/// <summary>
		/// Генератор условных операторов
		/// </summary>
		struct IfElseGeneration {
			CodeGeneration& parent; // Ссылка на внешнюю структуру

			IfElseGeneration(CodeGeneration& p) : parent(p) {}

			int IFLabelsCount = 0; // счетчик меток, используется дли уникального именования if-else меток
			int currentElseLabel = 0; // счетчик меток else, для хранения текущей метки else

			int nestingLevel = 0; // уровень вложенности, используется для отступов

			std::stack<std::string> if_stack;

			std::string cmp_op_to_jmp(std::string comparison);

			//void generate_condition__(
			//	const std::vector<std::string>& operands, // два операнда - левый и правый 
			//	const std::string& comparison, // операция сравнения (>, <, ==, !=, >=, <=)
			//	const std::string& trueLabel, // имя метки если условие выполняется
			//	const std::string& falseLabel, // если не выполняется
			//	std::vector<std::string>& instructions, bool isStringCmp = false
			//);

			void compare_ints(std::vector<std::string>& instructions, const vector<std::vector<int> >& operands);
			void compare_strings(std::vector<std::string>& instructions, const string& str1Name, const string& str2Name);

			void start_if__(const vector<int>& operands, // операция (>, <, ==, !=, >=, <=)
				std::vector<std::string>& // вектор с инструкциями, в которые будет добавлен сгенерированные новые
			);
			void start_else__(std::vector<std::string>& instructions);
			void end_if_or_else__(std::vector<std::string>& instructions);
			void end_expression__(std::vector<std::string>& instructions);

			std::vector<std::string> generate_if_statement(int& i);

			string generate_label(const string& prefix, int n) {
				return prefix + "_" + to_string(n);
			}
		} ifElseGen;
	};
#define BASE R"(.586
.model flat, stdcall
ExitProcess PROTO : DWORD
__PrintNumber PROTO :SDWORD
__PrintBool PROTO :BYTE
__PrintArray PROTO :DWORD, :DWORD, :DWORD
__Print PROTO :DWORD
__StrCmp PROTO :DWORD, :DWORD
.stack 4096)";
}