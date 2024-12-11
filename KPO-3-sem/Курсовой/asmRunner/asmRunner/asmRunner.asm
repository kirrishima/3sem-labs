.586
.model flat, stdcall
ExitProcess PROTO : DWORD
.stack 4096

.const
    __INT_L0 SDWORD 2
    __INT_L1 SDWORD 3

.data
    _@bool byte ? 
    _INT_PARAM_@sum__a sdword 0
    _INT_PARAM_@sum__b sdword 0
    _INT_PARAM_@product__a sdword 0
    _INT_PARAM_@product__b sdword 0


.code
sum proc
start:
    mov eax, [esp + 4]
    mov _INT_PARAM_@sum__a, eax
    mov eax, [esp + 8]
    mov _INT_PARAM_@sum__b, eax
    mov eax, _INT_PARAM_@sum__a
    mov ebx, _INT_PARAM_@sum__b
    add eax, ebx
sum_END:
    ret 8
sum endp

product proc
start:
    mov eax, [esp + 4]
    mov _INT_PARAM_@product__a, eax
    mov eax, [esp + 8]
    mov _INT_PARAM_@product__b, eax
    mov eax, _INT_PARAM_@product__a
    mov ebx, _INT_PARAM_@product__b
    imul ebx
product_END:
    ret 8
product endp

main proc
start:
    ; function call
    push __INT_L1
    push __INT_L0
    call product
    mov ebx, eax
    ; function call
    push __INT_L1
    push __INT_L0
    call sum
    imul ebx
    push eax

main_END:
    push 0
    call ExitProcess
main endp
    END main
