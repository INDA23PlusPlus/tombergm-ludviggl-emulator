#ifndef PRINTD_S
#define PRINTD_S

#include "div.S"

	; function: printd
	; prints the numer stored in bc
printd:
	push	psw
	push	bc
	push	de
	push	hl

	xra	a
	mov	h, a
	adi	10	; '\n'
	push	psw
	inr	h

printd_loop_1_:
	lxi	de, 10
	call	div
	mov	a, e
	adi	48	; '0'
	push	psw
	inr	h
	mov	a, b
	ora	c
	jnz	printd_loop_1_

printd_loop_2_:
	pop	psw
	out	0
	dcr	hl
	jnz	printd_loop_2_

	pop	hl
	pop	de
	pop	bc
	pop	psw
	ret

#endif
