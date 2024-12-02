.586
.model flat, stdcall
includelib kernel32.lib
ExitProcess PROTO : DWORD
WriteConsoleA PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
GetStdHandle PROTO :DWORD

STD_OUTPUT_HANDLE EQU -11  ; Дескриптор для стандартного вывода консоли

.stack 4096

.const
__L0 SDWORD 12
__L1 SDWORD 12
__L2 SDWORD 12
__L3 SDWORD 0
__L4 SDWORD 12
__L5 SDWORD 1

.data
	buffer db "result: ", 10 DUP(0)  ; Буфер для сообщения с результатом, включая место для числа
	bytesWritten DWORD 0               ; Переменная для записи количества байт
	___a SDWORD 0
	___piska SDWORD 0
	___x SDWORD 0


.code
; Процедура для преобразования числа в строку и записи в buffer
ConvertToString proc
    mov ecx, 10                     ; Основание системы счисления (десятичная система)
    mov edi, OFFSET buffer + 16     ; Указываем позицию для записи числа после "result: "
convertLoop:
    xor edx, edx                    ; Обнуляем edx перед делением
    div ecx                         ; Делим eax на 10, результат деления в eax, остаток в edx
    add dl, '0'                     ; Преобразуем остаток в ASCII
    dec edi                         ; Переходим к предыдущей позиции буфера
    mov [edi], dl                   ; Сохраняем символ в буфере
    test eax, eax                   ; Проверяем, остались ли ещё цифры
    jnz convertLoop                 ; Если остались, продолжаем цикл

    ; Добавляем символ новой строки и символ конца строки после числа
    mov byte ptr [OFFSET buffer + 17], 0Ah  ; Добавляем символ новой строки '\n'
    mov byte ptr [OFFSET buffer + 18], 0    ; Добавляем NULL в конец строки
    ret
ConvertToString endp

; Процедура для вывода строки в консоль
PrintToConsole proc
    ; Получаем дескриптор консоли
    push STD_OUTPUT_HANDLE
    call GetStdHandle
    mov ebx, eax                    ; Сохраняем дескриптор в ebx для использования в WriteConsoleA

    ; Вычисляем длину всей строки
    mov eax, OFFSET buffer + 18     ; Конец строки после новой строки и NULL
    sub eax, OFFSET buffer          ; Вычисляем общую длину строки от начала "result: " до конца числа

    ; Выводим строку в консоль
    push 0                          ; lpOverlapped (NULL)
    push OFFSET bytesWritten        ; lpNumberOfBytesWritten
    push eax                        ; nNumberOfBytesToWrite
    push OFFSET buffer              ; lpBuffer (начало всей строки "result: " + число)
    push ebx                        ; hConsoleOutput (дескриптор консоли)
    call WriteConsoleA              ; Вызываем WriteConsoleA для вывода строки
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
    ; Начало if
    ; Вычисляем значения операндов
    pop eax
    pop ebx
    ; Условие: ___a >=___piska
    cmp eax, ebx
    jge IF_TRUE_0
    jmp IF_END_0
IF_TRUE_0:
    mov eax,__L2
    mov ___x, eax
    

    mov eax, ___x
    call ConvertToString            ; Преобразуем число в строку
    call PrintToConsole             ; Выводим строку в консоль
        push __L3
        push ___a
        ; Начало if
        ; Вычисляем значения операндов
        pop eax
        pop ebx
        ; Условие: ___a =__L3
        cmp eax, ebx
        je IF_TRUE_1
        jmp ELSE_1
    IF_TRUE_1:
        mov eax, __L4
        call ConvertToString            ; Преобразуем число в строку
        call PrintToConsole             ; Выводим строку в консоль
        ; Переход к выходу из этого if-else блока
        jmp ELSE_1
    ELSE_1:
        mov eax, __L5
        call ConvertToString            ; Преобразуем число в строку
        call PrintToConsole             ; Выводим строку в консоль
        ; Переход к выходу из этого if-else блока
        jmp IF_END_1
    ELSE_1:
        mov eax, ___piska
        call ConvertToString            ; Преобразуем число в строку
        call PrintToConsole             ; Выводим строку в консоль
        ; Переход к выходу из этого if-else блока
        jmp IF_END_1
    IF_END_1:
    ; Переход к выходу из этого if-else блока
    jmp IF_END_0
IF_END_0:

; закончились условки
	push 0
	call ExitProcess
main ENDP
END main