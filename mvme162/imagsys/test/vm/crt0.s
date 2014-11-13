/*
 * $Id$			
 */
		move.l #0x400000,%sp

                jsr     main          
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


