.586
.model flat, stdcall
includelib libucrt.lib
includelib ../../Debug/GMSStandardLib.lib
ExitProcess PROTO : DWORD
__PrintNumber PROTO :SDWORD
__PrintBool PROTO :BYTE
__PrintArray PROTO :DWORD, :DWORD, :DWORD
__Print PROTO :DWORD
__StrCmp PROTO :DWORD, :DWORD
__PrintChar PROTO :DWORD
.stack 8192

.const
    __INT_L0 sword 1
    __INT_L1 sword 2
    __INT_L2 sword 10
    __INT_L3 sword -1
    __INT_L4 sword -7
    __INT_L5 sword 12
    __INT_L6 sword -5

.data
    @bool_RESERVED byte ? 
    _INT_PARAM_@sum__x sword 0
    _INT_PARAM_@sum__y sword 0


.code
sum proc
start:
    mov ax, [esp + 4]
    mov _INT_PARAM_@sum__x, ax
    mov ax, [esp + 6]
    mov _INT_PARAM_@sum__y, ax
    mov ax, _INT_PARAM_@sum__x
    mov bx, _INT_PARAM_@sum__y
    add ax, bx
sum_END:
    ret 4
sum endp

main proc
start:
    ; function call
    push __INT_L1
    push __INT_L0
    call sum
    push ax
    push __INT_L2
    pop bx
    pop ax
    imul bx
    push ax
    push __INT_L0
    push __INT_L3
    push __INT_L4
    pop bx
    pop ax
    imul bx
    push ax
    pop bx
    pop ax
    sub ax, bx
    push ax
    push __INT_L1
    pop bx
    pop ax
    imul bx
    push ax
    pop bx
    pop ax
    sub ax, bx
    push eax
    call __PrintNumber
    ; function call
    push __INT_L0
    push __INT_L5
    call sum
    push ax
    push __INT_L1
    pop bx
    pop ax
    imul bx
    push ax
    ; function call
    push __INT_L6
    push __INT_L3
    call sum
    push ax
    pop bx
    pop ax
    sub ax, bx
    push eax
    call __PrintNumber
    push 0
main_END:
    call ExitProcess
main endp
    END main
