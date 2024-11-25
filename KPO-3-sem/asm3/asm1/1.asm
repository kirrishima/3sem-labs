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

    mov ecx, 7                   	     ; Количество элементов массива
    lea esi, myWords                     ; Адрес массива в регистр ESI
    xor eax, eax                         ; Обнулить EAX (для суммы)
    mov ebx, 1                           ; Изначально EBX = 1 (нет нулевых элементов)
    
sum_loop:
    cmp ecx, 0                           ; Проверяем, есть ли ещё элементы
    je end_loop                          ; Если нет, выходим из цикла
     
    mov edx, [esi]                       ; Загружаем текущий элемент массива в EDX
    add eax, edx                         ; Добавляем значение элемента к сумме
    
    cmp edx, 0                           ; Проверяем, равен ли элемент 0
    je skip_zero                         ; Если равен, выходим из цикла

    add esi, 4                           ; Переходим к следующему элементу массива
    dec ecx                              ; Уменьшаем счётчик
    jmp sum_loop                         ; Повторяем цикл

skip_zero:
    mov ebx, 0                           ; Если равен 0, ставим EBX = 0

end_loop:
push 0
call ExitProcess

main ENDP
end  main