/*
 * RP
 * 18 / 11 / 98
 *
 * 
 *
 */
 
/*162Bug SYSCALL*/ 
RETURN	=	0x63	/*back to 162Bug*/
OUTSTR	=	0x21	/*sp address of first char, sp+4 addr of last char*/
INCHR	=	0x00	/*char returned in stack*/
/*ascii*/
CR	=	0x0D
LF	=	0x0A
	
 	.global main,debut
main:	move.l #12345,%d0
	lea.l msg_f,%a0
	subq.l #4,%a0
	
debut:	divu #10,%d0
	move.l %d0,%d1
	swap %d0
	add.b #'0',%d0
	move.b %d0,-(%a0)
	and.l #0xFFFF,%d1
	move.l %d1,%d0
	bne debut
 	|afficher une chaine de caractères
	pea.l msg_f
	pea.l msg_d
	|retour à 162bug
        TRAP #15
	.word OUTSTR
	
	TRAP #15
	.word RETURN

	
	
msg_d:	.ascii "le contenu de D0 est (base 10) "
	.ascii "                    "
	.byte CR,LF
msg_f:
	
	
