#ifndef MUL_S
#define MUL_S

	; function: mul
	; multiplies bc by de
	; returns the product in bc
mul:
	push	psw
	push	de
	push	hl

	lxi	hl, 0

mul_loop_:
	xra	a
	mov	a, b
	rrc
	mov	b, a
	mov	a, c
	rrc
	mov	c, a
	jnc	mul_nc_
	dad	de
mul_nc_:
	xra	a
	mov	a, e
	rlc
	mov	e, a
	mov	a, d
	rlc
	mov	d, a
	mov	a, b
	ora	c
	jnz	mul_loop_

	mov	b, h
	mov	c, l

	pop	hl
	pop	de
	pop	psw
	ret

#endif
