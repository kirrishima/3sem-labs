.586
.MODEL flat, stdcall
includelib kernel32.lib

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.STACK 4096

.CONST

.DATA
MB_OK EQU 0
STR1 DB "Assembly", 0
STR2 DB "mY FIRST � ����� ��� � ������ � �� ������ ����� �����", 0
HW DB ?

.CODE
main PROC
START: 
push MB_OK
push OFFSET STR1
push OFFSET STR2
push 0
CALL MessageBoxA

push 0
call ExitProcess

main ENDP
end main