	lxi	sp, 4096

	lxi	bc, 2
	call	printd

	lxi	bc, 3
prime_loop:
	call	is_prime
	jc	not_prime
	call	printd
not_prime:
	inx	bc
	mov	a, b
	ora	c
	jz	prime_loop_done
	inx	bc
	jmp	prime_loop
prime_loop_done:

	hlt

	; function: is_prime
	; checks if bc is prime for bc > 2
	; returns cy clear if prime, set otherwise
is_prime:
	push	psw
	push	bc
	push	de

	push	bc
	call	sqrt
	mov	h, b
	mov	l, c
	pop	bc

	lxi	de, 3
is_prime_loop_:
	mov	a, l
	sub	e
	mov	a, h
	sbb	d
	jc	is_prime_done_

	push	bc
	push	de
	call	div
	mov	a, d
	ora	e
	jnz	is_prime_nz_
is_prime_z_:
	pop	psw
	pop	psw
	pop	de
	pop	bc
	pop	psw
	stc
	ret
is_prime_nz_:
	pop	de
	pop	bc
	inx	de
	inx	de
	jmp	is_prime_loop_
is_prime_done_:

	pop	de
	pop	bc
	pop	psw
	stc
	cmc
	ret

#include "printd.S"
#include "sqrt.S"
