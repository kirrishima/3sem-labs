.586
.model flat, stdcall
ExitProcess PROTO : DWORD

.stack 4096

.const
    __INT_L0 SDWORD 10
    __STR_L1 db "x меньше 10", 0
    __STR_L2 db "x равно 10", 0
    __STR_L3 db "x больше 10", 0

.data
    _@bool byte ? 
    _INT_PARAM_@get_str__x sdword 0


.code
get_str proc
start:
    mov eax, [esp + 4]
    mov _INT_PARAM_@get_str__x, eax
    mov eax, _INT_PARAM_@get_str__x    ; lefthand операнд
    mov ebx, 10    ; righthand операнд
    
    cmp eax, ebx
    mov _@bool, 0
    jl @true0
    jmp @false0
    @true0:
    mov _@bool, 1
    @false0:
    ; Начало if0 (x<10) 
    cmp _@bool, 1
    je IF_TRUE_0
    jmp ELSE_0
IF_TRUE_0:
        mov eax, offset __STR_L1
        jmp get_str_END
        jmp IF_END_0    ; Переход к выходу из 0 if-else
ELSE_0:
        mov eax, _INT_PARAM_@get_str__x    ; lefthand операнд
        mov ebx, 10    ; righthand операнд
    
        cmp eax, ebx
        mov _@bool, 0
        je @true1
        jmp @false1
        @true1:
        mov _@bool, 1
        @false1:
        ; Начало if1 (x=10) 
        cmp _@bool, 1
        je IF_TRUE_1
        jmp ELSE_1
    IF_TRUE_1:
            jmp IF_END_1    ; Переход к выходу из 1 if-else
    ELSE_1:
            jmp IF_END_1    ; Переход к выходу из 1 if-else
    IF_END_1:
        jmp IF_END_0    ; Переход к выходу из 0 if-else
IF_END_0:
get_str_END:
    ret 4
get_str endp

main proc
start:
    push __INT_L0
    call get_str
    push eax
main_END:
    push 0
    call ExitProcess
main endp
    END main
