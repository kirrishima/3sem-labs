.586
.model flat, stdcall
ExitProcess PROTO : DWORD

.stack 4096

.data
;Parsed ints: 
_SDW0    sdword    1
_SDW1    sdword    2
_SDW2    sdword    2
_SDW3    sdword    3
_SDW4    sdword    4
_SDW5    sdword    55
_SDW6    sdword    6
_SDW7    sdword    7
_SDW8    sdword    8
_SDW9    sdword    9

;Parsed strings: 
_ByteArray0    byte    'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\0'
_ByteArray1    byte    'r', 'e', 'f', 'd', 's', ' ', 'g', 'f', 'd', ' ', 'r', 'e', 'g', ' ', '3', '4', '2', '3', '4', ' ', 'g', 'f', 'y', '6', '5', 'y', ' ', '-', '=', '0', '3', '0', '2', '9', '\0'


.CODE
main PROC
START:
	push 0
	call ExitProcess

	;Some code...

main ENDP
end  main