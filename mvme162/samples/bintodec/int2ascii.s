/* int2ascii.s
 *
 *
 * Tue Nov 18 00:30:13 CET 2003 RP
 * 	- compteur avec appel de fonction
 * 
 * 18 / 11 / 98 RP
 * 	- version initiale
 * 
 *
commandes de compilation
m68k-ELF-linux-as int2ascii.s -o int2ascii.o
m68k-ELF-linux-ld -oformat=srec -Ttext=0x10000 int2ascii.o -o int2ascii
cat int2ascii>/dev/ttyS0

saisie écran exécution 
162-Bug>lo 0

162-Bug>
162-Bug>
        g
Effective address: 00010000
compteur 0 à 100 V0.0.0
0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899bye....
162-Bug>
 */
 
/*162Bug SYSCALL*/ 
RETURN	=	0x63	/*back to 162Bug*/
OUTSTR	=	0x21	/*sp address of first char, sp+4 addr of last char*/
INCHR	=	0x00	/*char returned in stack*/
/*ascii*/
CR	=	0x0D
LF	=	0x0A


	/* 
	 * programme principal
	 */
 	.global _start
_start:	
	|initialisation pointeur de pile
	move.l #0x01000000,%sp
	
 	|afficher une chaine de caractères de présentation
	pea.l msg1_f
	pea.l msg1_d
        TRAP #15
	.word OUTSTR
	
	|boucle de comptage
	move.b #0,%d0
l1:	
	move.l #0x12000,%a0	|adresse où mettre les car. du nombre
	move.l %d0,-(%sp)	|sauvegarde d0 sur la pile
	jsr bin2dec
	move.l (%sp)+,%d0
	
	move.l %a1,-(%sp)	|afficher le nombre
	move.l %a0,-(%sp)
        TRAP #15
	.word OUTSTR
	
	addq.b #1,%d0
	cmp.b #100,%d0
	bne l1
	
	pea.l msg2_f
	pea.l msg2_d
        TRAP #15
	.word OUTSTR
	
	|retour à 162bug
	TRAP #15
	.word RETURN
	
	/* fonction de conversion int (16bits non signé) -> ascii
	 * entrée : 
	 *	D0 : entier
	 *	A0 : adresse disponible pour caractères
	 * sortie :
	 *	A0 : début
	 *	A1 : fin de chaine représentant le nombre
	 */	
bin2dec:
	move.l %a0,%a1
l2:	divu #10,%d0
	move.l %d0,%d1
	swap %d0
	add.b #'0',%d0
	move.b %d0,-(%a0)
	and.l #0xFFFF,%d1
	move.l %d1,%d0
	bne l2
	rts

	/* 
	 * chaine de caractères du programme
	 */
msg1_d:	.ascii "compteur 0 à 100 V0.0.0"
	.byte CR,LF
msg1_f:	
msg2_d:	.ascii "bye...."
	.byte CR,LF
msg2_f:
	
