#ifndef DIV_S
#define DIV_S

	; function: div
	; performs integer division of bc by de
	; returns quotient in bc, remainder in de
div:
	push	psw
	push	hl

	xra	a
	mov	h, a
	mov	l, a

	stc
	cmc

	mvi	a, 16

div_loop_:
	push	psw

	mov	a, c
	rlc
	mov	c, a
	mov	a, b
	rlc
	mov	b, a
	mov	a, l
	rlc
	mov	l, a
	mov	a, h
	rlc
	mov	h, a

	push	hl

	mov	a, l
	sub	e
	mov	l, a
	mov	a, h
	sbb	d
	mov	h, a

	jnc	div_nc_
div_c_:
	pop	hl
	pop	psw
	dcr	a
	stc
	cmc
	jnz	div_loop_
	jmp	div_done_
div_nc_:
	pop	psw
	pop	psw
	dcr	a
	stc
	jnz	div_loop_
div_done_:

	mov	a, c
	rlc
	mov	c, a
	mov	a, b
	rlc
	mov	b, a

	mov	e, l
	mov	d, h

	pop	hl
	pop	psw
	ret

#endif
