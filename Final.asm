include macros2.asm 
include number.asm 


.MODEL LARGE
.386
.STACK 200h

MAXTEXTSIZE equ 40

.DATA
	_13	dd	13.0
	_B_MAYOR_IGUAL	db	'B_MAYOR_IGUAL','$', 13 dup (?)
	_B_MENOR	db	'B_MENOR','$', 7 dup (?)
	_31_33	dd	31.33
	_5	dd	5.0
	_3	dd	3.0
	_9	dd	9.0
	_2	dd	2.0
	g	db	MAXTEXTSIZE dup (?),'$'
	f	dd	?
	e	dd	?
	d	dd	?
	c	dd	?
	b	dd	?
	a	dd	?
	_SUM	dd	?
	_SUB	dd	?
	_MUL	dd	?
	_DIV	dd	?


.CODE
STRLEN PROC
	mov bx,0
STRL01:
	cmp BYTE PTR [SI+BX],'$'
	je STREND
	inc BX
	cmp BX, MAXTEXTSIZE
	jl STRL01
STREND:
	ret
STRLEN ENDP

COPY PROC
	call STRLEN
	cmp bx,MAXTEXTSIZE
	jle COPYSIZEOK
	mov bx,MAXTEXTSIZE
COPYSIZEOK:
	mov cx,bx
	cld
	rep movsb
	mov al,'$'
	mov BYTE PTR [DI],al
	ret
COPY ENDP

	begin: .startup

	; Simple Asignation
	FLD _2
	FSTP b

	; Simple Asignation
	FLD _2
	FSTP c

	; Simple Asignation
	FLD _2
	FSTP d

	; Simple Asignation
	FLD _2
	FSTP f

	; ADD
	FLD _9
	FLD b
	FADD
	FSTP _SUM

	; SUB
	FLD _SUM
	FLD c
	FSUB
	FSTP _SUB

	; DIV
	FLD _SUB
	FLD _3
	FDIV
	FSTP _DIV

	; MUL
	FLD _DIV
	FLD _5
	FMUL
	FSTP a

	; Simple Asignation
	FLD _31_33
	FSTP e

	; Condition
	FLD f
	FCOMP e
	FSTSW AX
	SAHF
	JNE IF_0

	; Condition
	FLD d
	FCOMP b
	FSTSW AX
	SAHF
	JNE IF_0

	; Condition
	FLD c
	FCOMP a
	FSTSW AX
	SAHF
	JNE IF_0

	; PRINT
	displayString _B_MENOR
	newLine 1

	; Condition
	FLD b
	FCOMP c
	FSTSW AX
	SAHF
	JB IF_1

	; PRINT
	displayString _B_MAYOR_IGUAL
	newLine 1

	; Simple Asignation
	FLD _13
	FSTP b


IF_1:

IF_0:

	; FREE STACK
	FFREE st(0)
	FFREE st(1)
	FFREE st(2)
	FFREE st(3)
	FFREE st(4)
	FFREE st(5)
	FFREE st(6)
	FFREE st(7)



;END PROGRAM
	mov AX, 4C00h
	int 21h

	END begin