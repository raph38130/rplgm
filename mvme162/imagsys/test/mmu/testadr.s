/******************************************************************************/
/**                                                                          **/
/** @(#) title:                                                             >**/
/** @(#) keywords:                                                          >**/
/**                                                                          **/
/**   Appel : testadr(faultadr, funccode);                                   **/
/**     faultadr : Fault Address                                             **/
/**     funccode : champ TM dans le Special Status Word                      **/
/**                                                                          **/
/******************************************************************************/

/*
   description de la pile
*/
Lfaultadr =       8
Lfunccode =       12

	xdef	testadr
testadr:
	link    %a6,#0

	move.l	Lfunccode(%a6),%d0
	movec	%d0,%dfc
	movea.l	Lfaultadr(%a6),%a0
	ptestr	(%a0)
	movec	%mmusr,%d0

	unlk    %a6
	rts
