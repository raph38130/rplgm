/* demo.s
 * Fri Mar 10 09:47:59 CET 2006 RP
 *
 * transfert d'une zone m�moire dans MVME162
 * 
 	A1 d�but
	A2 fin
	A3 destination
	
	D0 utilis� comme interm�diaire
	
	ne g�re pas les recouvrements
 *
 */

loop:
	cmp.l %a1,%a2
	beq fin
	move.b (%a1),%d0
	move.b %d0,(%a3)
	add.l #1,%a1
	add.l #1,%a3
	bra loop
	
	
fin:	trap #15
	dc.w 0x63
	
