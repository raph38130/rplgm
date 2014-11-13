/*
 * $Id$
 *			
 */
		.globl _start
_start:
		move.l #0x1000000,%a7
		           
                jsr     main            /* Call prog startup */
/*
 * main return to 162Bug
 * 
 */
                trap #15
		.word 0x63

/*
 * cstart reduit ...
 */
		.globl __main
__main:
		rts


