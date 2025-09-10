; It adds two number together, waow
; https://schweigi.github.io/assembler-simulator/index.html

	JMP start
number1: DB 84	; first number
number2: DB 54	; second number
initialSP: DB 0	; init position of SP, to check when its empty

start:
	MOV [initialSP], sp
	MOV D, 232		; D = output
	MOV C, [number1]	; C = value of number1
	ADD C, [number2]	; C = result of addition
getDigit:			
	MOV A, C	; A = result
	MOV B, C	; B = result
	CMP C, 10	; check if smaller than 10
	JB addDigit	; if so, skip modulo stuff
	DIV 10		; A (result) / 10
	MUL 10		; (A/10) * 10
	
	SUB B, A	; B = result - ((A/B) * B) = result % 10
			; Now B has the last digit
addDigit:
	ADD B, '0'	; Add '0' to get hexadecimal value
	PUSH B		; Push number onto stack
	MOV A, C	; Copy result to A
	DIV 10		; Divide by 10 to remove number we pushed
	MOV C, A	; Copy back to C
	CMP C, 0	; check if we are done
	JNZ getDigit	; Go back if not
printDigit:
	CMP SP, [initialSP]	; check if any left
	JZ end			; if not, end
	POP A			; pop next digit from stack
	MOV [D], A		; move to output
	INC D			; increment output pointer
	JMP printDigit		; loop
end:
	HLT
