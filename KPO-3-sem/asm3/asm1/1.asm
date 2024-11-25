.586P													
.MODEL      FLAT, STDCALL
includelib  kernel32.lib

ExitProcess PROTO :DWORD
MessageBoxA PROTO :DWORD, :DWORD, :DWORD, :DWORD		

.STACK      4096

.CONST													

.DATA				
    a dword 1
    b dword 3
    myWords DWORD 1,2,3,4,6,6,7

.CODE													

main PROC
START:													
    mov esi, offset myWords
    mov ax,  [esi + 4]
    mov bx,  [esi + 2]

    mov ecx, 7                   	     ; ���������� ��������� �������
    lea esi, myWords                     ; ����� ������� � ������� ESI
    xor eax, eax                         ; �������� EAX (��� �����)
    mov ebx, 1                           ; ���������� EBX = 1 (��� ������� ���������)
    
sum_loop:
    cmp ecx, 0                           ; ���������, ���� �� ��� ��������
    je end_loop                          ; ���� ���, ������� �� �����
     
    mov edx, [esi]                       ; ��������� ������� ������� ������� � EDX
    add eax, edx                         ; ��������� �������� �������� � �����
    
    cmp edx, 0                           ; ���������, ����� �� ������� 0
    je skip_zero                         ; ���� �����, ������� �� �����

    add esi, 4                           ; ��������� � ���������� �������� �������
    dec ecx                              ; ��������� �������
    jmp sum_loop                         ; ��������� ����

skip_zero:
    mov ebx, 0                           ; ���� ����� 0, ������ EBX = 0

end_loop:
push 0
call ExitProcess

main ENDP
end  main