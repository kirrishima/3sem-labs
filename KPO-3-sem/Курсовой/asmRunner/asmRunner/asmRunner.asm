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
__L1 SDWORD 12
__L2 SDWORD 12
__L3 SDWORD 0
__L4 SDWORD 12
__L5 SDWORD 1

.data
	buffer db "result: ", 10 DUP(0)  ; ����� ��� ��������� � �����������, ������� ����� ��� �����
	bytesWritten DWORD 0               ; ���������� ��� ������ ���������� ����
	___a SDWORD 0
	___piska SDWORD 0
	___x SDWORD 0


.code
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

main proc
start:
	mov eax,__L0
	mov ___a, eax
	

	push ___a
	push __L1
	pop ebx
	pop eax
	imul ebx
	push eax
	pop eax
	mov ___piska, eax
	

    push ___piska
    push ___a
    ; ������ if
    ; ��������� �������� ���������
    pop eax
    pop ebx
    ; �������: ___a >=___piska
    cmp eax, ebx
    jge IF_TRUE_0
    jmp IF_END_0
IF_TRUE_0:
    mov eax,__L2
    mov ___x, eax
    

    mov eax, ___x
    call ConvertToString            ; ����������� ����� � ������
    call PrintToConsole             ; ������� ������ � �������
        push __L3
        push ___a
        ; ������ if
        ; ��������� �������� ���������
        pop eax
        pop ebx
        ; �������: ___a =__L3
        cmp eax, ebx
        je IF_TRUE_1
        jmp ELSE_1
    IF_TRUE_1:
        mov eax, __L4
        call ConvertToString            ; ����������� ����� � ������
        call PrintToConsole             ; ������� ������ � �������
        ; ������� � ������ �� ����� if-else �����
        jmp ELSE_1
    ELSE_1:
        mov eax, __L5
        call ConvertToString            ; ����������� ����� � ������
        call PrintToConsole             ; ������� ������ � �������
        ; ������� � ������ �� ����� if-else �����
        jmp IF_END_1
    ELSE_1:
        mov eax, ___piska
        call ConvertToString            ; ����������� ����� � ������
        call PrintToConsole             ; ������� ������ � �������
        ; ������� � ������ �� ����� if-else �����
        jmp IF_END_1
    IF_END_1:
    ; ������� � ������ �� ����� if-else �����
    jmp IF_END_0
IF_END_0:

; ����������� �������
	push 0
	call ExitProcess
main ENDP
END main