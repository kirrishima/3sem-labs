.586
.model flat, stdcall
ExitProcess PROTO : DWORD
__PrintNumber PROTO : SDWORD

.stack 4096

.const
	__L0 SDWORD 12
	__L1 SDWORD 12
	__L2 SDWORD 12
	__L3 SDWORD 1
	__L4 SDWORD 8
	__L5 SDWORD 122

.data
	___a SDWORD 0
	___piska SDWORD 0


.code

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
    ; �������: ___a <___piska
    cmp eax, ebx
    jl IF_TRUE_0
    jmp ELSE_0
IF_TRUE_0:
        push __L2
        push ___a
        ; ������ if
        ; ��������� �������� ���������
        pop eax
        pop ebx
        ; �������: ___a =__L2
        cmp eax, ebx
        je IF_TRUE_1
        jmp ELSE_1
    IF_TRUE_1:
        push ___a
        call __PrintNumber  ; ������� ����� � �������
        ; ������� � ������ �� ����� if-else �����
        jmp IF_END_1
    ELSE_1:
        push __L3
        call __PrintNumber  ; ������� ����� � �������
        ; ������� � ������ �� ����� if-else �����
        jmp IF_END_1
    IF_END_1:
    push __L4
    call __PrintNumber  ; ������� ����� � �������
    ; ������� � ������ �� ����� if-else �����
    jmp IF_END_0
ELSE_0:
    push ___piska
    call __PrintNumber  ; ������� ����� � �������
    ; ������� � ������ �� ����� if-else �����
    jmp IF_END_0
IF_END_0:

; ����������� �������
	push ___a
	push __L5
	pop ebx
	pop eax
	imul ebx
	push eax
	pop eax
	mov ___a, eax
	

	push ___a
	call __PrintNumber  ; ������� ����� � �������
	push 0
	call ExitProcess
main ENDP
END main