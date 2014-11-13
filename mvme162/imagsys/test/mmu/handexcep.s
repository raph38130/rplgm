	xdef	handexcep
	.globl	SP_exception
handexcep:
	move.l	%a7,SP_exception
	movem.l	%d0-%d7/%a0-%a6,-(%a7)
	move.l	%usp,%a0
	move.l %a0,-(%a7)

	jsr Exception

	move.l (%a7)+,%a0
	move.l	%a0,%usp
	movem.l	(%a7)+,%d0-%d7/%a0-%a6
	rte
