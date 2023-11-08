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

#include "printd.S"
