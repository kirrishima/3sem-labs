#pragma once
#include <string>
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <format>
#include <stack>
#include "Parm.h"

using namespace std;

string trim(const string& str);
bool isMASMLabel(const string& line);
string removeComment(const string& str);
bool isValidIdentifier(const string& identifier);
bool isFunctionStart(const string& line);
bool isFunctionEnd(const string& line);

string operator*(const string& str, int times);

namespace CD
{
	bool is_assignment(const string& expr);

	struct UserDefinedFunctions;

	struct CodeGeneration {
		string tab = "    ";
		const string reservedBoolName = "_@bool";
		int trueLabelsCount = 0;

		IT::ID_Table ID_TABLE;
		LT::LexTable LEX_TABLE;
		std::ofstream OUT_ASM_FILE;
		Parm::PARM parm;

		std::unordered_map<string, UserDefinedFunctions> user_functions;

		CodeGeneration(const IT::ID_Table& ID_TABLE, const LT::LexTable& LEX_TABLE, const Parm::PARM& parm)
			: ID_TABLE(ID_TABLE), LEX_TABLE(LEX_TABLE), parm(parm), ifElseGen(*this)
		{
			this->OUT_ASM_FILE.open(parm.masmDest);
		}

		static string get_id_name_in_data_segment(const IT::Entry& entry);
		vector<string> __parse_math_expression(const vector<int>& expression);
		vector<string> generate_math_expressions(const vector<int>& expr);

		void __s_const();
		void __s_data();

		string lexems_vector_to_source_string(const vector<int>& ids);
		string lexem_to_source(LT::Entry& entry);

		vector<string> parse_function_call(UserDefinedFunctions& function, int params_start_index, int params_end_index);
		void parse_function(int start_index, int end_index);
		void parse_function_body(UserDefinedFunctions& function, int start_index, int end_index);

		void parse_lexem(std::vector<std::string>& result_instructions, int& index_in_lex_table);
		void parse_lexem_equal__(std::vector<std::string>& result_instructions, int& index_in_lex_table);

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
			string stringRepresentation;
			string resultStorage;
		};

		/// <summary>
		/// выражение присваивани€, параметры функции print, услови€  
		/// </summary>
		/// <param name="ids"></param>
		/// <param name="instructions"></param>
		/// <returns></returns>
		ParseExpressionReturnParms parse_expression(vector<int> ids, vector<string>& instructions);
		void parse_print_lexem__(std::vector<std::string>& result_instructions, int& i);

		/// <summary>
		/// ¬озвращает строку, представл€ющую собой текущее значение строковой переменной в виде имени литерала
		/// </summary>
		/// <param name="lex_id"></param>
		/// <returns></returns>
		string get_string_value(const int lex_id);
		//// <summary>
		/// ѕреобразует входной вектор индексов лексем в строку, представл€ющую выражение с именами на асм.
		/// »спользуетс€ дл€ генерации польской натации по выражению
		/// </summary>
		/// <param name="ids">индексы лесем</param>
		/// <returns></returns>
		string lexems_vector_to_string(const vector<int>& ids);

		void gen(const std::wstring& OUT_FILEPATH);

		/// <summary>
		/// √енератор условных операторов
		/// </summary>
		struct IfElseGeneration {
			CodeGeneration& parent; // —сылка на внешнюю структуру

			IfElseGeneration(CodeGeneration& p) : parent(p) {}

			int IFLabelsCount = 0; // счетчик меток, используетс€ дли уникального именовани€ if-else меток
			int currentElseLabel = 0; // счетчик меток else, дл€ хранени€ текущей метки else
			int nestingLevel = 0; // уровень вложенности, используетс€ дл€ отступов

			std::stack<string> if_stack;

			string cmp_op_to_jmp(string comparison);

			//void generate_condition__(
			//	const vector<string>& operands, // два операнда - левый и правый 
			//	const string& comparison, // операци€ сравнени€ (>, <, ==, !=, >=, <=)
			//	const string& trueLabel, // им€ метки если условие выполн€етс€
			//	const string& falseLabel, // если не выполн€етс€
			//	vector<string>& instructions, bool isStringCmp = false
			//);

			void compare_ints(vector<string>& instructions, const vector<vector<int> >& operands);
			void compare_strings(vector<string>& instructions, const string& str1Name, const string& str2Name);

			void __start_if(const vector<int>& operands, // операци€ (>, <, ==, !=, >=, <=)
				vector<string>& // вектор с инструкци€ми, в которые будет добавлен сгенерированные новые
			);

			void __start_else(vector<string>& instructions);
			void __end_if_or_else(vector<string>& instructions);
			void __end_expression(vector<string>& instructions);

			vector<string> generate_if_statement(int& i);

			string generate_label(const string& prefix, int n) {
				return prefix + "_" + to_string(n);
			}
		} ifElseGen;
	};


	struct UserDefinedFunctions
	{
		string name;
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

#define BASE R"(.586
.model flat, stdcall
ExitProcess PROTO : DWORD
__PrintNumber PROTO :SDWORD
__PrintBool PROTO :BYTE
__PrintArray PROTO :DWORD, :DWORD, :DWORD
__Print PROTO :DWORD
__StrCmp PROTO :DWORD, :DWORD)";
}