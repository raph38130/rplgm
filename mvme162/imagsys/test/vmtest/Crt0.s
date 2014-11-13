/*
 * $Id$
 *
 * projet Système 1998
 *
 * memory map	
 * -=-=-=-=-=
 *	Mem.	|	nom		droits S	droits d'un process mode U
 *		|
 *	$09000	|	environ
 *		|
 *      $10000	|	kernel 		r(w)x 		---
 *		|
 *	$15000	|	user		n'accède pas	r(w)x
 *
 *		|	func.o		n'accède pas	--x
 *
 *
 * pour chaque process :
 *	pile USP			---		rw
 *
 *	pile SSP			rw		ne doit pas avoir accès (sauf par trap)
 *
 * et enfin :
 *	pile système			rw		---
 *			
 */
		move.l #0x400000,%a6
		move.l #0x400000,%a7
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


