/* blinkauto.s
 *
 * Fri Mar 17 09:49:25 CET 2006 RP
 *
 *	version avec les interruptions
 *
 * Mon Mar 13 13:47:05 CET 2006 RP
 *
 *	clignotant avec la led rouge mvme162
 *
 
m68k-linux-as blinkauto.s -o blinkauto.o
m68k-linux-ld blinkauto.o -o blinkauto.srec --oformat=srec -Ttext=0x10000

 */
VBR =	0x0 		/* 0xFE000000*/

RSCR = 	0xFFF42044	/*led rouge*/
T1CMP = 0xFFF42004	/*T1 compare register*/
T1CR =	0xFFF42017	/*T1 cntrl register*/
T1ICR = 0xFFF4201B	/*T1 interrupt cr*/
 
	/* main loop */

	move.L #1000000,T1CMP	/*cycle 1s*/
	move.b #0x03,T1CR	/*CEN COC*/
	move.b #0x13,T1ICR	/*IEN lev=3*/

	/* installer vecteur */
	move.l isr, VBR + 4 * (0x50 + 0x09)

	/* demasquer les it */
	move.w #0x2000,%sr
loop: 
	bra loop
	
	
	/* ISR */
isr:
	BTST #1,RSCR
	beq suite1
	BSET #1,RSCR
	bra suite2
suite1:	BCLR #1,RSCR
	
suite2:	/* acquiter IT */
	move.b #0x1B,T1ICR	/*IEN ICLR level3*/ 

	RTE
	
	
