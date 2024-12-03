.386
.model flat, stdcall
option casemap:none

extern HeapAlloc@12:PROC
extern HeapFree@12:PROC
extern GetProcessHeap@0:PROC
ExitProcess PROTO :dword

.data
    buffer_size dd 64                     ; ������ ������
    heap_handle dd 0                      ; ���������� ����
    string_ptr dd 0                       ; ��������� �� ���������� ������
    message db "Dynamic memory allocation example", 0

.code
main proc
start:
    ; ��������� ����������� ����
    call GetProcessHeap@0
    mov heap_handle, eax                  ; ��������� ���������� ����

    ; ��������� ������
    mov eax, heap_handle
    mov ecx, buffer_size
    push 0                                ; ��������� ������ (0 - ����������� ���������)
    push ecx                              ; ������ ������
    push eax                              ; ���������� ����
    call HeapAlloc@12                     ; ����� HeapAlloc
    mov string_ptr, eax                   ; ��������� ��������� �� ���������� ������

    ; ����������� ������ � ���������� ������
    lea esi, message                      ; ����� ������
    mov edi, string_ptr                   ; ����� ���������� ������
copy_loop:
    lodsb                                 ; �������� ����� �� [ESI] � AL
    stosb                                 ; ������ ����� �� AL � [EDI]
    test al, al                           ; �������� ����� ������
    jnz copy_loop

    ; ������������ ������
    mov eax, heap_handle
    mov ecx, string_ptr
    push 0                                ; ����� (0 - ����������� ���������)
    push ecx                              ; ����� ������ ��� ������������
    push eax                              ; ���������� ����
    call HeapFree@12                      ; ����� HeapFree

    ; ���������� ���������
    push 0
    call ExitProcess
main ENDP
END main