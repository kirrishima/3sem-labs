.586
.model flat, stdcall
INCLUDELIB <E:\3-sem\KPO-3-sem\Курсовой\18\Debug\GMSStandardLib.lib>
ExitProcess PROTO : DWORD
EXTERN __PrintNumber : PROC
EXTERN __PrintBool : PROC
EXTERN __PrintArray : PROC
EXTERN __StrCmp : PROC
.stack 4096

PrintArrayMACRO MACRO arrName
    LOCAL arrType, arrLength, arrOffset
    push offset arrName   ; Смещение массива
    call __PrintArray     ; Вызов процедуры __PrintArray
ENDM

StrCmpCallMACRO MACRO str1, str2
    push OFFSET str2   ; Адрес второй строки в стек
    push OFFSET str1   ; Адрес первой строки в стек
    call __StrCmp      ; Вызов функции __StrCmp
ENDM

.const
    __L0 SDWORD 12
    __L1 db "18", 0

.data
    _@bool byte ? 
    _PARAM__aboba__x sdword 0
    _STR_PTR__aboba__y dword ?
    _main__x sdword 0


.code
aboba proc
start:
    mov eax, [esp + 4]
    mov _PARAM__aboba__x, eax
    mov eax, [esp + 8]
    mov _STR_PTR__aboba__y, eax
    push _PARAM__aboba__x
    call __PrintNumber
    PrintArrayMACRO _STR_PTR__aboba__y
    ret 8
aboba endp

main proc
start:
    mov eax, __L0
    mov _main__x, eax
    push offset __L1
    push _main__x
    call aboba
    push 0
    call ExitProcess
main endp
    END main
