	lxi	sp, 4096

	lxi	bc, 0
	lxi	de, 1

fib_loop:
	call	printd
	push	de
	push	bc
	pop	hl
	dad	de
	xchg
	pop	bc
	jnc	fib_loop
	call	printd

	hlt

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
