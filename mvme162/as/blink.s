/* blink.s
 *
 * Mon Mar 13 13:47:05 CET 200 RP
 *
 *	clignotant avec la led rouge mvme162
 *
 
m68k-linux-as blink.s -o blink.o
m68k-linux-ld blink.o -o blink.srec --oformat=srec -Ttext=0x10000
 */

/* registre de controle du reset bit n°1 led rouge */
RSCR = 0xFFF42044

loop:
	/* led 0N */
	BSET #1,RSCR
	
	/* wait */
	move.l #0xF,%D1
l0:	move.l #0xFFFFFFFF,%D0
l1:     dbra %D0,l1
	dbra %D1,l0
	
	/* led OFF */
	BCLR #1,RSCR
	
	/* wait */
	move.l #0xF,%D1
l2:	move.l #0xFFFFFF,%D0
l3:     dbra %D0,l3
	dbra %D1,l2

	bra loop
