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
	__L1 SDWORD 100
	__L2 SDWORD 13
.data
buffer db "result: ", 10 DUP(0)  ; Буфер для сообщения с результатом, включая место для числа
bytesWritten DWORD 0             ; Переменная для записи количества байт
	a SDWORD 0
	f SDWORD 0
	x SDWORD 0
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
call ConvertToString            ; Преобразуем число в строку
call PrintToConsole             ; Выводим строку в консоль
	push 0
	call ExitProcess
main ENDP
end main