#pragma once
#include <string>
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <format>
#include <stack>
#include "Parm.h"

using namespace std;

bool isMASMLabel(const string& line);
string removeComment(const string& str);
bool isValidIdentifier(const string& identifier);
bool isFunctionStart(const string& line);
bool isFunctionEnd(const string& line);
int get_id_size_in_bytes(IT::IDDATATYPE type);
int getEscapeCode(const std::string& input);

string operator*(const string& str, int times);

namespace CD
{
	bool is_assignment(const string& expr);

	struct UserDefinedFunctions;

	struct ExceptionsNames;

	struct CodeGeneration {
		string tab = "    ";
		const string reservedBoolName = "@bool_RESERVED";
		int trueLabelsCount = 0;

		IT::ID_Table ID_TABLE;
		LT::LexTable LEX_TABLE;
		std::ofstream OUT_ASM_FILE;
		const Parm::PARM* parm;

		UserDefinedFunctions* currentFunction = nullptr;

		std::unordered_map<string, UserDefinedFunctions*> user_functions;
		std::vector<UserDefinedFunctions*> __user_functions;

		CodeGeneration(const IT::ID_Table& ID_TABLE, const LT::LexTable& LEX_TABLE, const Parm::PARM* parm)
			: ID_TABLE(ID_TABLE), LEX_TABLE(LEX_TABLE), parm(parm), ifElseGen(*this)
		{
			this->OUT_ASM_FILE.open(parm->masmDest);
		}

		static string get_id_name_in_data_segment(const IT::Entry& entry);
		vector<string> __parse_math_expression(const vector<int>& expression);
		vector<string> generate_math_expressions(const vector<int>& expr);

		void __s_const();
		void __s_data();

		string lexems_vector_to_source_string(const vector<int>& ids);
		string lexem_to_source(LT::Entry& entry);

		vector<string> parse_function_call(UserDefinedFunctions* function, int params_start_index, int params_end_index);
		void parse_function(int start_index, int end_index);
		void parse_function_body(UserDefinedFunctions* function, int start_index, int end_index);

		void parse_lexem(std::vector<std::string>& result_instructions, int& index_in_lex_table, int tabsize = 0);
		void parse_lexem_equal__(std::vector<std::string>& result_instructions, int& index_in_lex_table, int tabsize = 0);

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
			bool isCHAR = false;
			bool isFunctionCall = false;

			IT::IDDATATYPE returnDataType;
			string stringRepresentation;
			string resultStorage;
		};

		/// <summary>
		/// ��������� ������������, ��������� ������� print, �������  
		/// </summary>
		/// <param name="ids"></param>
		/// <param name="instructions"></param>
		/// <returns></returns>
		ParseExpressionReturnParms parse_expression(vector<int> ids, vector<string>& instructions, int tabsize = 0);
		void parse_print_lexem__(std::vector<std::string>& result_instructions, int& i, int tabsize = 0);

		/// <summary>
		/// ���������� ������, �������������� ����� ������� �������� ��������� ���������� � ���� ����� ��������
		/// </summary>
		/// <param name="lex_id"></param>
		/// <returns></returns>
		string get_string_value(const int lex_id);
		//// <summary>
		/// ����������� ������� ������ �������� ������ � ������, �������������� ��������� � ������� �� ���.
		/// ������������ ��� ��������� �������� ������� �� ���������
		/// </summary>
		/// <param name="ids">������� �����</param>
		/// <returns></returns>
		string lexems_vector_to_string(const vector<int>& ids);

		void generateCode(const std::wstring& OUT_FILEPATH);

		/// <summary>
		/// ��������� �������� ����������
		/// </summary>
		struct IfElseGeneration {
			CodeGeneration& parent; // ������ �� ������� ���������

			IfElseGeneration(CodeGeneration& p) : parent(p) {}

			int IFLabelsCount = 0; // ������� �����, ������������ ��� ����������� ���������� if-else �����
			int currentElseLabel = 0; // ������� ����� else, ��� �������� ������� ����� else
			int nestingLevel = 0; // ������� �����������, ������������ ��� ��������

			std::stack<string> if_stack;

			string cmp_op_to_jmp(string comparison);

			//void generate_condition__(
			//	const vector<string>& operands, // ��� �������� - ����� � ������ 
			//	const string& comparison, // �������� ��������� (>, <, ==, !=, >=, <=)
			//	const string& trueLabel, // ��� ����� ���� ������� �����������
			//	const string& falseLabel, // ���� �� �����������
			//	vector<string>& instructions, bool isStringCmp = false
			//);

			void compare_ints(vector<string>& instructions, const vector<vector<int> >& operands);
			void compare_strings(vector<string>& instructions, const string& str1Name, const string& str2Name);

			void __start_if(const vector<int>& operands, // �������� (>, <, ==, !=, >=, <=)
				vector<string>& // ������ � ������������, � ������� ����� �������� ��������������� �����
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
		string endLabel;
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

	struct ExceptionsNames {
		static const string DivideByZero;
	};

#define BASE R"(.586
.model flat, stdcall
ExitProcess PROTO : DWORD
__PrintNumber PROTO :SDWORD
__PrintBool PROTO :BYTE
__PrintArray PROTO :DWORD, :DWORD, :DWORD
__PrintStr PROTO :DWORD
__PrintChar PROTO :DWORD

__StreamWriteNumber PROTO :SDWORD
__StreamWriteBool PROTO :BYTE
__StreamWriteArray PROTO :DWORD, :DWORD, :DWORD
__StreamWriteStr PROTO :DWORD
__StreamWriteChar PROTO :DWORD

__StrCmp PROTO :DWORD, :DWORD

__NullDevisionException PROTO)";
}