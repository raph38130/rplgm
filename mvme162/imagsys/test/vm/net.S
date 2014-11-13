NETRD	=	0x18
NETWR	=	0x19
NETCTRL	=	0x1d
/*
 * call to NETRD 16xBug Function 5-25
 */
	 .even
.globl	netrd_16xbug

netrd_16xbug:
	move.l	4(%a7),-(%a7)
	trap	#15
	.word	NETRD
	bne	Lerreur
	clr.l	%d0
	rts

/*
 * call to NETWR 16xBug Function 5-25 ------------------------------------------
 */
	.even
.globl	netwr_16xbug

netwr_16xbug:
	move.l	4(%a7),-(%a7)
	trap	#15
	.word	NETWR
	bne	Lerreur
	clr.l	%d0
	rts

/*
 * call to NETCTRL 16xBug Function 5-38 ----------------------------------------
 */
	.even
.globl	netcrtl_16xbug
netcrtl_16xbug:
	move.l	4(%a7),-(%a7)
	trap	#15
	.word	NETCTRL
	bne	Lerreur
	clr.l	%d0
	rts

/*
 * Point erreur   --------------------------------------------------------------
 */
Lerreur:
	move.l	#-1,%d0
	rts

