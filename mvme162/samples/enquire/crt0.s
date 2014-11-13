/*
 * $Id$
 *			
 */
		move.l #0x400000-10,%a7
		move.l #0x400000-10,%a6
               jsr     main            /* Call e2s startup */
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


