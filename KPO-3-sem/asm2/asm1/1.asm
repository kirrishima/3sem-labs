.586P													
.MODEL FLAT, STDCALL									
includelib kernel32.lib									

ExitProcess PROTO :DWORD								
MessageBoxA PROTO :DWORD, :DWORD, :DWORD, :DWORD		

.STACK 4096										

.CONST													

.DATA				
		a dword 1
		b dword 3
		str0 db "питса", 0 
		str1 db "Результат сложения = < >", 0 

.CODE													

main PROC												
START:													
		mov eax, a									    
		add eax, b										
		add eax, '0' 								   
		
		mov str1+22, al									
		
		invoke MessageBoxA, 0, offset str1, offset str0, 0

		push 0											
		call ExitProcess								
main ENDP

end main