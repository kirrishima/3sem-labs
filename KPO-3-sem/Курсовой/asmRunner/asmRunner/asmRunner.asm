.586
.model flat, stdcall
ExitProcess PROTO : DWORD
.stack 4096

.const
    __INT_L0 sword 12
    __INT_L1 sword 1
    __INT_L2 sword 2
    __INT_L3 sword -1
    __INT_L4 sword -5

.data
    _@bool byte ? 
    _INT_PARAM_@sum__x sword 0
    _INT_PARAM_@sum__y sword 0


.code
sum proc
start:
    mov ax, [esp + 2]
    mov _INT_PARAM_@sum__x, ax
    mov ax, [esp + 4]
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
    ; function call
    push __INT_L4
    push __INT_L3
    call sum
    push ax
    pop bx
    pop ax
    sub ax, bx
    push ax

main_END:
    push 0
    call ExitProcess
main endp
    END main
