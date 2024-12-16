.686P
.model flat, stdcall
includelib ../../Debug/GMSStandardLib.lib
includelib libucrt.lib

ExitProcess PROTO :DWORD     ; �������� ExitProcess
__StreamWriteStr PROTO :DWORD
__NullDevisionException PROTO

.data
    dividend DWORD 100         ; �������
    divisor DWORD 0            ; ��������
    errMsg BYTE "Division by zero detected. Exiting program...", 0

.code
main PROC
    ; ��������� �������� ����� ��������
    mov eax, divisor           ; ��������� ��������
    cmp eax, 0                 ; ���������, ����� �� �������� 0
    je division_error          ; ���� ��, ��������� �� ��������� ������

    ; ���������� �������
    mov eax, dividend          ; ��������� ������� � EAX
    xor edx, edx               ; ������� ������� ����
    div dword ptr divisor      ; ��������� �������

    ; ���������� ���������
    invoke ExitProcess, 0

division_error:
    call __NullDevisionException
    ; ��������� ������ ������� �� ����
    push OFFSET errMsg         ; ��������� ��������� �� ������
    call __StreamWriteStr      ; ������� ���������
    add esp, 4                 ; ������������� �����
    invoke ExitProcess, 1      ; ���������� ��������� � ����� ������

main ENDP
END main

