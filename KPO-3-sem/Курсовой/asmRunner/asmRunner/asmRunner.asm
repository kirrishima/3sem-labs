.386
.model flat, stdcall
option casemap:none

extern HeapAlloc@12:PROC
extern HeapFree@12:PROC
extern GetProcessHeap@0:PROC
ExitProcess PROTO :dword

.data
    buffer_size dd 64                     ; Размер буфера
    heap_handle dd 0                      ; Дескриптор кучи
    string_ptr dd 0                       ; Указатель на выделенную память
    message db "Dynamic memory allocation example", 0

.code
main proc
start:
    ; Получение дескриптора кучи
    call GetProcessHeap@0
    mov heap_handle, eax                  ; Сохраняем дескриптор кучи

    ; Выделение памяти
    mov eax, heap_handle
    mov ecx, buffer_size
    push 0                                ; Параметры флагов (0 - стандартное поведение)
    push ecx                              ; Размер памяти
    push eax                              ; Дескриптор кучи
    call HeapAlloc@12                     ; Вызов HeapAlloc
    mov string_ptr, eax                   ; Сохраняем указатель на выделенную память

    ; Копирование строки в выделенную память
    lea esi, message                      ; Адрес строки
    mov edi, string_ptr                   ; Адрес выделенной памяти
copy_loop:
    lodsb                                 ; Загрузка байта из [ESI] в AL
    stosb                                 ; Запись байта из AL в [EDI]
    test al, al                           ; Проверка конца строки
    jnz copy_loop

    ; Освобождение памяти
    mov eax, heap_handle
    mov ecx, string_ptr
    push 0                                ; Флаги (0 - стандартное поведение)
    push ecx                              ; Адрес памяти для освобождения
    push eax                              ; Дескриптор кучи
    call HeapFree@12                      ; Вызов HeapFree

    ; Завершение программы
    push 0
    call ExitProcess
main ENDP
END main