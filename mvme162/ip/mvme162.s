/* 
 * BC, RP
 * 
 * points d'entrées dans 162-Bug
 *
 * Wed Jul  5 22:21:08 CEST 2000
 */

INCHR   =       0

/*******************************************************************************
#   char inchr()
#
#******************************************************************************/

        .globl  inchr
inchr:

        link    %a6,#0
	
        subq.l  #2,%sp
        trap    #15
        .word   INCHR
        move.b  (%sp)+,%d0

        unlk    %a6
        rts

OUTSTR  =       0x21
OUTLN   =       0x22

/*******************************************************************************
#   void outstr(deb,fin)
#   char *deb;   /*   adresse debut de la chaine      */
#   char *fin;   /*   adresse fin de la chaine + 1    */
#
#
#   la description de la pile
#******************************************************************************/
#
deb     =       8
fin     =       12

        .globl  outstr
outstr:
        link    %a6,#0
        move.l  fin(%a6),-(%sp)
        move.l  deb(%a6),-(%sp)
        trap    #15
        .word   OUTSTR

        unlk    %a6
        rts

/*******************************************************************************
#   void outln(deb,fin)
#   char *deb;   /*   adresse debut de la chaine      */
#   char *fin;   /*   adresse fin de la chaine + 1    */
#
#******************************************************************************/
/*
   la description de la pile
   (la meme que pour outsrt)
*/

        .globl  outln
outln:
        link    %a6,#0
        move.l  fin(%a6),-(%sp)
        move.l  deb(%a6),-(%sp)
        trap    #15
        .word   OUTLN

        unlk    %a6
        rts
	
	
NETRD   =       0x18
NETWR   =       0x19
NETCTRL =       0x1d
/*
 * call to NETRD 16xBug Function 5-25
 */
         .even
.globl  netrd_16xbug

netrd_16xbug:
        move.l  4(%a7),-(%a7)
        trap    #15
        .word   NETRD
        bne     Lerreur
        clr.l   %d0
        rts

/*
 * call to NETWR 16xBug Function 5-25 ------------------------------------------
 */
        .even
.globl  netwr_16xbug

netwr_16xbug:
        move.l  4(%a7),-(%a7)
        trap    #15
        .word   NETWR
        bne     Lerreur
        clr.l   %d0
        rts

/*
 * call to NETCTRL 16xBug Function 5-38 ----------------------------------------
 */
        .even
.globl  netcrtl_16xbug
netcrtl_16xbug:
        move.l  4(%a7),-(%a7)
        trap    #15
        .word   NETCTRL
        bne     Lerreur
        clr.l   %d0
        rts

/*
 * Point erreur   --------------------------------------------------------------
 */
Lerreur:
        move.l  #-1,%d0
        rts
/*
   les points d'entree de 10xBug
*/
RETURN  =       0x63


/*******************************************************************************
#   void ret16xbug()
#
#   status est ignore
#******************************************************************************/

        .globl  ret16xbug
ret16xbug:

        trap    #15
        .word   RETURN
        rts
	
/*
   les points d'entre de 10xBug
*/
TM_INI  =       0x40
TM_STR0 =       0x41
TM_RD   =       0x42
DELAY   =       0x43

/*******************************************************************************
#   void tm_ini()
#
#******************************************************************************/

        .globl  tm_ini
tm_ini:

        trap    #15
        .word   TM_INI
        rts

/*******************************************************************************
#   void tm_str0()
#
#******************************************************************************/

        .globl  tm_str0
tm_str0:

        trap    #15
        .word   TM_STR0

        rts

/*******************************************************************************
#   int tm_rd()
#
#******************************************************************************/

        .globl  tm_rd
tm_rd:
        link    %a6,#0

        subq.l  #4,%sp          /*   space for result   */
        trap    #15
        .word   TM_RD
        move.l  (%sp)+,%d0

        unlk    %a6
        rts
/*******************************************************************************
#   void delay(delta)
#   int delta;   
#
#******************************************************************************/
/*
   la description de la pile
*/
delta   =       8

        .globl  delay
delay:
        link    %a6,#0

        move.l  delta(%a6),-(%sp)
        trap    #15
        .word   delta
	
        unlk    %a6
        rts





