.586P													; система подкоманд(процессор Pentium)
.MODEL FLAT, STDCALL									; модель памяти, соглашение о вызове
includelib kernel32.lib									; компановщику: компоновать с kernel32

ExitProcess PROTO :DWORD								; прототип функции для завершения процесса Windows
MessageBoxA PROTO :DWORD, :DWORD, :DWORD, :DWORD		; прототип API-функции MessageBoxA

.STACK 4096										

.CONST													

.DATA				
		a dword 4
		b dword 3
		str0 db "питса", 0 
		str1 db "Результат сложения = < >", 0 

.CODE													

main PROC												
START :													
		mov eax, a									    ; загрузка в регистр eax значения переменной a
		add eax, b										; сложение значений переменных a и b
		add eax, 30h 								    ; сложение 30h с результатом сложения
		
		mov str1+22, al									;младшая половина подрегистра
		
		invoke MessageBoxA, 0, offset str1, offset str0, 0

		push 0											;код возврата процесса Windows (параметр ExitProcess)
		call ExitProcess								;так завершается любой процесс Windows
main ENDP

end main