.686P
.model flat, stdcall
includelib ../../Debug/GMSStandardLib.lib
includelib libucrt.lib

ExitProcess PROTO :DWORD     ; Прототип ExitProcess
__StreamWriteStr PROTO :DWORD
__NullDevisionException PROTO

.data
    dividend DWORD 100         ; Делимое
    divisor DWORD 0            ; Делитель
    errMsg BYTE "Division by zero detected. Exiting program...", 0

.code
main PROC
    ; Проверяем делитель перед делением
    mov eax, divisor           ; Загружаем делитель
    cmp eax, 0                 ; Проверяем, равен ли делитель 0
    je division_error          ; Если да, переходим на обработку ошибки

    ; Выполнение деления
    mov eax, dividend          ; Загружаем делимое в EAX
    xor edx, edx               ; Очищаем старшие биты
    div dword ptr divisor      ; Выполняем деление

    ; Завершение программы
    invoke ExitProcess, 0

division_error:
    call __NullDevisionException
    ; Обработка ошибки деления на ноль
    push OFFSET errMsg         ; Загружаем сообщение об ошибке
    call __StreamWriteStr      ; Выводим сообщение
    add esp, 4                 ; Корректировка стека
    invoke ExitProcess, 1      ; Завершение программы с кодом ошибки

main ENDP
END main

