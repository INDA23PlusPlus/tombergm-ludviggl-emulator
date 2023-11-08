#ifndef SQRT_S
#define SQRT_S

#include "mul.S"

	; function: sqrt
	; computes the square root of bc
	; returns the result in bc
sqrt:
	push	psw
	push	de
	push	hl

	xra	a
	mov	h, a
	mov	l, a
	mov	d, a
	stc
	rrc
	mov	e, a

sqrt_loop_:
	push	hl
	dad	de

	push	de
	push	bc
	push	hl
	pop	bc
	push	hl
	pop	de
	call	mul
	pop	de
	push	de
	mov	a, e
	sub	c
	mov	a, d
	sbb	b
	jc	sqrt_c_
	pop	bc
	pop	de
	pop	psw
	jmp	sqrt_nc_
sqrt_c_:	
	pop	bc
	pop	de
	pop	hl
sqrt_nc_:
	stc
	cmc
	mov	a, e
	rrc
	mov	e, a
	jnc	sqrt_loop_

	mov	b, h
	mov	c, l

	pop	hl
	pop	de
	pop	psw
	ret

#endif
