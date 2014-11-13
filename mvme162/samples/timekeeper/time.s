/*
 * RP
 * 18 / 11 / 98
 *
 * lecture du TIME KEEPER
 *
 * les données sont codées en BCD
 *
 */
 
/*Intel TIME KEEPER*/
SECONDS	=	0xFFFC1FF9
MINUTES	=	0xFFFC1FFA
HOUR	=	0xFFFC1FFB
DAY	=	0xFFFC1FFC
DATE	=	0xFFFC1FFD
MONTH	=	0xFFFC1FFE
YEAR	=	0xFFFC1FFF
/*162Bug SYSCALL*/ 
RETURN	=	0x63	/*back to 162Bug*/
OUTSTR	=	0x21	/*sp address of first char, sp+4 addr of last char*/
INCHR	=	0x00	/*char returned in stack*/
/*ascii*/
CR	=	0x0D
LF	=	0x0A

	.global main
	
main:	lea.l suite,%a0
	lea.l HOUR,%a1	
	jsr read_time
	move.b #':',(%a0)+
	lea.l MINUTES,%a1		
	jsr read_time
	move.b #':',(%a0)+
	lea.l SECONDS,%a1		
	jsr read_time
	move.b #' ',(%a0)+	
	move.b #'l',(%a0)+
	move.b #'e',(%a0)+
	move.b #' ',(%a0)+
	lea.l DATE,%a1		
	jsr read_time
	move.b #'/',(%a0)+
	lea.l MONTH,%a1		
	jsr read_time
	move.b #'/',(%a0)+
	lea.l YEAR,%a1		
	jsr read_time

	move.b #CR,(%a0)+
	move.b #LF,(%a0)+
	move.l %a0,-(%a7)
 	|afficher la chaine de caractères
	move.l %a0,-(%a7)
	pea.l debut
	trap #15
	.word OUTSTR
	|retour à 162bug
        TRAP #15
	.word RETURN
	
/* en entrée :	A1 registre du TimeKeeper à lire
 * en sortie :	D1 code ascii du chiffre des dizaines
 *		D0 code ascii du chiffre des unités
 */
read_time:
	move.b (%a1),%d0		
	move.b %d0,%d1		
	lsr.b #4,%d1		
	and.b #0xF,%d1		
	add.b #'0',%d1		
	and.b #0xF,%d0		
	add.b #'0',%d0		
	move.b %d1,(%a0)+
	move.b %d0,(%a0)+
	rts

debut:	.ascii "timekeeper indique "
suite:
	
	
