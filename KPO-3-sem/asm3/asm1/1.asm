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
		str1 db "Результат сложения = < >", 0
		str0 db "питса", 0
		myWords WORD 8Ah, 44h, 3Bh, 5Fh, 99h

.CODE													

main PROC
START:													
		mov esi, offset myWords
		mov ax, [esi + 4]
		mov bx, [esi + 2]

		invoke MessageBoxA, 0, offset str1, offset str0, 0

		push 0
		call ExitProcess
main ENDP

end  main