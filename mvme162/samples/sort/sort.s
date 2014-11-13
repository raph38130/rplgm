/*
 * RP
 * Wed Feb  3 18:16:09 MET 1999
 *
 * tri par insertion d'une liste d'entiers
 * trace :
 *
 *162-Bug>g                                                                       
 *Effective address: 00010000                                                     
 *                                                                               
 *5  9  7  0  4  12  5  2  5  123  0  12345  12                                   
 *                                                                               
 *0  0  2  4  5  5  5  7  9  12  12  123  12345                                   
 *
 */
 
/*162Bug SYSCALL*/ 
RETURN	=	0x63	/*back to 162Bug*/
OUTSTR	=	0x21	/*sp address of first char, sp+4 addr of last char*/
INCHR	=	0x00	/*char returned in stack*/
/*ascii*/
CR	=	0x0D
LF	=	0x0A


/*
 * NL (new line)
 * macro générant un saut de ligne
 */
	.macro NL	
	pea.l nl_f
	pea.l nl_d
        TRAP #15
	.word OUTSTR	
	.endm
	
/*
 * prog principal
 */
 	.global main
main:	
	|avant
	move.l #13,%d0
	lea.l liste_d,%a0
	jsr visu
	
	move.l #13,%d0
	lea.l liste_d,%a0
	jsr tri
	
	|après
	move.l #13,%d0
	lea.l liste_d,%a0
	jsr visu
	
	TRAP #15
	.word RETURN
	

/*
 * tri
 * tri par insertion d'une liste d'entiers.W
 * en entrée 	D0.B	nombre d'entiers
 *		A0	début de la liste
 * 	A1 fin de liste
 *	D0 ppe potentiel
 *	A3 index
 *	A2 adresse du ppe
 */
tri:	
	movem.l %a0-%a3/%d0,-(%a7)
	move.l %a0,%a1
	lsl.l #1,%d0		|*2 car .w
        add.l %d0,%a1    	|charge fin de liste dans A1

depart: 			
	CMP.L   %a0,%a1         |a-t-on trié toute la liste ?
        BEQ fin_tri
        MOVE.L  %a0,%a3
        
        |recherche du ppe entre A3 et A1
        MOVE.W (%a3),%d0 
	move.l %a3,%a2       
loop_tri:
        addq.l #2,%a3
	CMP.L   %a3,%a1         
        BEQ     fin_recher_ppe
        CMP.W   (%a3),%d0       
        BLT     loop_tri	|pas un nouveau ppe
        MOVE.W  (%a3),%d0
        MOVE.L  %a3,%a2
        BRA loop_tri

        |echange du ppe situé en A2 et A0
fin_recher_ppe:
        MOVE.W (%a0),%d0
        MOVE.W  (%a2),(%a0) 
	move.w %d0,(%a2)
	
        ADDQ.L   #2,%a0         |ajoute 2 à A0
        BRA     depart          |branchement à debut

fin_tri:
	movem.l (%a7)+,%a0-%a3/%d0
	rts
	
/*
 * visu
 * affiche une liste d'entiers.W
 * en entrée 	D0.B	nombre d'entiers
 *		A0	début de la liste
 *
 *
 *	A0,A1,A2,D0,D1 modifiés
 *
 */
visu:
	NL
	move.l %a0,%a2
	move.b %d0,%d1
loop:	beq fin
	move.w (%a2)+,%d0
	lea.l buffer_d,%a0
	jsr bintodec
	move.l %a1,-(%a7)
	move.l %a0,-(%a7)
        TRAP #15
	.word OUTSTR
	pea.l sep_f
	pea.l sep_d
        TRAP #15
	.word OUTSTR	
	subq.b #1,%d1
	bra loop
	
fin:	
	NL
	rts	
	
	
/*
 * bintodec
 * en entrée 	D0	entier.W non signé
 *		A0	adresse d'un buffer (8 octets) où mettre les chiffres
 *
 * en sortie	A0	début des chiffres
 *		A1	fin des chiffres
 *
 * 
 */
bintodec:
	addq.l #8,%a0
	move.l %a0,%a1	
	move.l %d1,-(%a7)	|sauvegarde D1 utilisé ci dessous
	
debut:	divu #10,%d0
	move.l %d0,%d1
	swap %d0
	add.b #'0',%d0
	move.b %d0,-(%a0)
	and.l #0xFFFF,%d1
	move.l %d1,%d0
	bne debut
	
	move.l (%a7)+,%d1	|restauration de D1
	rts

/*
 * la liste originale
 */
liste_d:
	dc.w 5,9,7,0,4,12,5,2,5,123,0,12345,12
liste_f:


/* 
 * quelques buffer utiles
 */	
buffer_d:	
	.ascii "         "
buffer_f:
	
	
sep_d:	.ascii "  "
sep_f:  .ascii "  "

nl_d:	dc.b CR,LF
nl_f:  


	

