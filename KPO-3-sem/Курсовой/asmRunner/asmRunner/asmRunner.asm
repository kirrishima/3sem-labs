.586
.model flat, stdcall
ExitProcess PROTO :DWORD
__PrintNumber PROTO :SDWORD
__PrintArray PROTO :SDWORD, :SDWORD, :SDWORD

.stack 4096

.data
    ; �������� �������
    myArray SDWORD 1, 2, 3, 4, 5   ; ������ � 5 ����������
    arraySize SDWORD 5              ; ������ �������

.code
main proc
    push type myArray
    push lengthof myArray
    push offset myArray
    call __PrintArray
    ret
main endp

END main
