.586
.model flat, stdcall
ExitProcess PROTO : DWORD

.stack 4096


.const
	__L0 SDWORD 0
	__L1 SDWORD 1
	__L2 db "st", 0
	__L3 db "str", 0
	__L4 db "строки равны", 0

.data
	___a SDWORD 0
	___s dword ?
	___ss dword ?


.code

main proc
start:
	mov eax, __L0
	mov ebx, __L1
	sub eax, ebx
	mov ___a, eax
	
	mov eax, -1
    cmp eax, 0    ; Сравнение знаковое
    jl IF_TRUE_0  ; Переход, если меньше нуля
    jmp IF_END_0
IF_TRUE_0:
    jmp IF_END_0    ; Переход к выходу из 0 if-else
IF_END_0:
    push eax
; закончились условки
	push 0
	call ExitProcess
main ENDP
END main