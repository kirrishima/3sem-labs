#pragma once
#include <string>
#include "LT.h"
#include "IT.h"

namespace CD
{
	void gen(const LT::LexTable& LEX_TABLE, const IT::ID_Table& ID_TABLE, const std::wstring& OUT_FILEPATH, bool);

	void __generate_math_expressions(const std::string& expr, std::ofstream& outFile);

	const std::string printAsmCode =
		"; ��������� ��� �������������� ����� � ������ � ������ � buffer\n"
		"ConvertToString proc\n"
		"    mov ecx, 10                     ; ��������� ������� ��������� (���������� �������)\n"
		"    mov edi, OFFSET buffer + 16     ; ��������� ������� ��� ������ ����� ����� \"result: \"\n"
		"convertLoop:\n"
		"    xor edx, edx                    ; �������� edx ����� ��������\n"
		"    div ecx                         ; ����� eax �� 10, ��������� ������� � eax, ������� � edx\n"
		"    add dl, '0'                     ; ����������� ������� � ASCII\n"
		"    dec edi                         ; ��������� � ���������� ������� ������\n"
		"    mov [edi], dl                   ; ��������� ������ � ������\n"
		"    test eax, eax                   ; ���������, �������� �� ��� �����\n"
		"    jnz convertLoop                 ; ���� ��������, ���������� ����\n\n"
		"    ; ��������� ������ ����� ������ � ������ ����� ������ ����� �����\n"
		"    mov byte ptr [OFFSET buffer + 17], 0Ah  ; ��������� ������ ����� ������ '\\n'\n"
		"    mov byte ptr [OFFSET buffer + 18], 0    ; ��������� NULL � ����� ������\n"
		"    ret\n"
		"ConvertToString endp\n\n"
		"; ��������� ��� ������ ������ � �������\n"
		"PrintToConsole proc\n"
		"    ; �������� ���������� �������\n"
		"    push STD_OUTPUT_HANDLE\n"
		"    call GetStdHandle\n"
		"    mov ebx, eax                    ; ��������� ���������� � ebx ��� ������������� � WriteConsoleA\n\n"
		"    ; ��������� ����� ���� ������\n"
		"    mov eax, OFFSET buffer + 18     ; ����� ������ ����� ����� ������ � NULL\n"
		"    sub eax, OFFSET buffer          ; ��������� ����� ����� ������ �� ������ \"result: \" �� ����� �����\n\n"
		"    ; ������� ������ � �������\n"
		"    push 0                          ; lpOverlapped (NULL)\n"
		"    push OFFSET bytesWritten        ; lpNumberOfBytesWritten\n"
		"    push eax                        ; nNumberOfBytesToWrite\n"
		"    push OFFSET buffer              ; lpBuffer (������ ���� ������ \"result: \" + �����)\n"
		"    push ebx                        ; hConsoleOutput (���������� �������)\n"
		"    call WriteConsoleA              ; �������� WriteConsoleA ��� ������ ������\n"
		"    ret\n"
		"PrintToConsole endp\n";
}