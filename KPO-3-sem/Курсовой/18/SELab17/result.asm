.586
.model flat, stdcall
includelib kernel32.lib
ExitProcess PROTO : DWORD
WriteConsoleA PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
GetStdHandle PROTO :DWORD

STD_OUTPUT_HANDLE EQU -11  ; ���������� ��� ������������ ������ �������

.stack 4096
.const
	__L0 SDWORD 12
	__L1 SDWORD 100
	__L2 SDWORD 13
.data
buffer db "result: ", 10 DUP(0)  ; ����� ��� ��������� � �����������, ������� ����� ��� �����
bytesWritten DWORD 0             ; ���������� ��� ������ ���������� ����
	a SDWORD 0
	f SDWORD 0
	x SDWORD 0
; ��������� ��� �������������� ����� � ������ � ������ � buffer
ConvertToString proc
    mov ecx, 10                     ; ��������� ������� ��������� (���������� �������)
    mov edi, OFFSET buffer + 16     ; ��������� ������� ��� ������ ����� ����� "result: "
convertLoop:
    xor edx, edx                    ; �������� edx ����� ��������
    div ecx                         ; ����� eax �� 10, ��������� ������� � eax, ������� � edx
    add dl, '0'                     ; ����������� ������� � ASCII
    dec edi                         ; ��������� � ���������� ������� ������
    mov [edi], dl                   ; ��������� ������ � ������
    test eax, eax                   ; ���������, �������� �� ��� �����
    jnz convertLoop                 ; ���� ��������, ���������� ����

    ; ��������� ������ ����� ������ � ������ ����� ������ ����� �����
    mov byte ptr [OFFSET buffer + 17], 0Ah  ; ��������� ������ ����� ������ '\n'
    mov byte ptr [OFFSET buffer + 18], 0    ; ��������� NULL � ����� ������
    ret
ConvertToString endp

; ��������� ��� ������ ������ � �������
PrintToConsole proc
    ; �������� ���������� �������
    push STD_OUTPUT_HANDLE
    call GetStdHandle
    mov ebx, eax                    ; ��������� ���������� � ebx ��� ������������� � WriteConsoleA

    ; ��������� ����� ���� ������
    mov eax, OFFSET buffer + 18     ; ����� ������ ����� ����� ������ � NULL
    sub eax, OFFSET buffer          ; ��������� ����� ����� ������ �� ������ "result: " �� ����� �����

    ; ������� ������ � �������
    push 0                          ; lpOverlapped (NULL)
    push OFFSET bytesWritten        ; lpNumberOfBytesWritten
    push eax                        ; nNumberOfBytesToWrite
    push OFFSET buffer              ; lpBuffer (������ ���� ������ "result: " + �����)
    push ebx                        ; hConsoleOutput (���������� �������)
    call WriteConsoleA              ; �������� WriteConsoleA ��� ������ ������
    ret
PrintToConsole endp

.code
main proc
start:
	mov eax, __L0
	mov [a], eax

	; original expression:  a+__L1
	mov eax, [a]
	add eax, __L1
	mov [f], eax

	; original expression:  a+f
	mov eax, [a]
	add eax, [f]
	mov [x], eax

	; original expression:  __L2+x
	mov eax, __L2
	add eax, [x]
	mov [f], eax
	mov eax, [f]
call ConvertToString            ; ����������� ����� � ������
call PrintToConsole             ; ������� ������ � �������
	push 0
	call ExitProcess
main ENDP
end main