
; NRV2D DECOMPRESSOR FOR INTEL 8080 (SHORTER AND SLOW VERSION)
; IN:	DE - PACKED DATA SOURCE ADDRESS
;	HL - UNPACKED DATA DESTINATION ADDRESS
; OUT:	DE - POINTS TO THE POSITION AFTER LAST BYTE OF INPUT DATA
;	HL - POSITION AFTER LAST BYTE OF OUTPUT BUFFER
; STACK USAGE: 8 BYTES (4 WORDS) + 2 BYTES (1 WORD) RETURN ADDRESS
; COMPILED SIZE: 182 BYTES (18% SLOWER THAN FAST VERSION)

; IMPORTANT. THIS CODE DOES NOT ACCEPT ORIGINAL PACKED BYTE SEQUENCES'S FINAL
; MARKER. 32-BIT OFFSET USED IN ORIGINAL ALGORITHM IS DIFFICULT TO ENCODE ON
; 8-BIT PROCESSOR, THE COMPRESSION SUBROUTINE `ucl_nrv_99_compress` MUST BE
; MODIFICATED TO STORE SHORT FINAL CODE PREFIX AND WORK PROPERLY:
; UCL VERSION: 1.03, FILE `src\n2_99.ch`, LINE #605, ORIGINAL:
;	code_prefix_ss12(c, UCL_UINT32_C(0x1000000));
; SHOULD BE:
;	code_prefix_ss12(c, UCL_UINT32_C(0x100));


N2DUNPK:
	LXI	B, -1
	PUSH	B
	MVI	A, 80H
	JMP	DCL

NXDC:	MOV	B, A
	LDAX	D
	MOV	M, A
	INX	D
	INX	H
	MOV	A, B
DCL:	CALL	RB
	JC	NXDC

	PUSH	H	; SP: DST -LAST_M_OFF
	LXI	H, 1	; M_OFF
OFN:	DAD	H
	CALL	RB
	JNC	OV1
	INX	H
OV1:	CALL	RB
	JC	OFE
	DCX	H
	DAD	H
	CALL	RB
	JNC	OFN
	INX	H
	JMP	OFN

RB:	ADD	A
	RNZ
	LDAX	D
	INX	D
	ADC	A
	RET

OFE:	MOV	B, A
	DCX	H
	DCX	H
	MOV	A, L
	ORA	H
	DCX	H
	JNZ	OF2

	MOV	A, B
	POP	B
	POP	H
	PUSH	H
	PUSH	B
	PUSH	H	; SP: -M_OFF DST -LAST_M_OFF
	CALL	RB
	JMP	OF3

OF2:	MOV	H, L
	LDAX	D
	INX	D
	CMA
	MOV	L, A
	MOV	A, H
	CMA
	MOV	H, A
	ORA	L
	JZ	BRK

	MOV	A, H
	STC
	RAR
	MOV	H, A
	MOV	A, L
	RAR
	MOV	L, A

	MOV	A, B
	POP	B
	INX	SP
	INX	SP
	PUSH	H
	PUSH	B
	PUSH	H
; SP: -M_OFF DST -LAST_M_OFF

OF3:	MOV	C, H
	LXI	H, 0
	JNC	OF4
	INX	H

OF4:	DAD	H
	CALL	RB
	JNC	OV3
	INX	H
OV3:	MOV	B, A
	MOV	A, L
	ORA	H
	JNZ	LF1
	INX	H
	MOV	A, B
NXLF:	DAD	H
	CALL	RB
	JNC	OV4
	INX	H
OV4:	CALL	RB
	JNC	NXLF
	INX	H
	INX	H
	MOV	B, A
LF1:	MOV	A, C
	CPI	0FBH
	JNC	NOG
	INX	H
NOG:	INX	H

; HL = M_LEN  SP: -M_OFF DST -LAST_M_OFF
	MOV	A, B
	MOV	B, H
	MOV	C, L
	POP	H	; HL = - M_OFF
	XCHG		; DE = - M_OFF; HL = SRC
	XTHL		; HL = DST
	PUSH	H	; SP: DST SRC
	DAD	D	; HL = DST - M_OFF
	POP	D	; DE = DST;  SP: SRC
	PUSH	PSW

NXMV:	MOV	A, M
	STAX	D
	INX	D
	INX	H
	DCX	B
	MOV	A, C
	ORA	B
	JNZ	NXMV

	POP	PSW
	XCHG
	POP	D
	JMP	DCL

BRK:	POP	H
	POP	PSW
	RET

END
