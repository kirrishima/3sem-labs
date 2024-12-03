.586
.model flat, stdcall
ExitProcess PROTO :DWORD
__PrintNumber PROTO :SDWORD
__PrintArray PROTO :SDWORD, :SDWORD, :SDWORD

.stack 4096

.data
    ; Создание массива
    myArray SDWORD 1, 2, 3, 4, 5   ; Массив с 5 элементами
    arraySize SDWORD 5              ; Размер массива

.code
main proc
    push type myArray
    push lengthof myArray
    push offset myArray
    call __PrintArray
    ret
main endp

END main
