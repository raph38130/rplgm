/*
 * PROJET SYSTEME : 1997/1998
 * context-switch.s
 * $Id$
 * échange deux contextes en 2 * 3 étapes :
 *	1) sauvegarde de USP
 *	2) sauvegarde des registres sur SSP
 *	3) sauvegarde de SSP
 *	suivi d'une restauration du nouveau contexte
 *
 * 28 / 02 / 98 modification pour terminer par RTE (afin que la VM soit implémentable)
 *
 * QUESTION faut-il faire un stack adjust ?
 */

|sur la pile, il y a : 
|     old_context 
|     new_context
|
|emplacement de SP et USP dans le process descriptor
|doit coller à "sys_create()" dans "kernel/create.c")
   .set _SP,	0x0
   .set _USP,	0x4
   .set _U_SRP,	0x8
   .globl switch_context
switch_context:

|%a0 repere old_context
move.l 4(%sp),%a0
|%d0 repere new_context
move.l 8(%sp),%d0

|sauvegarde de "old_context"
|on conserve USP
move.l %usp,%a1
move.l %a1,_USP(%a0)

|les registres non sauvegardés lors du trap sont mis sur la pile système
movem.l %d2-%d7/%a2-%a6,-(%sp)

|si coprocesseur flottant, sauvegardes supplémentaires à faire ci-dessous

|on conserve SP
move.l %sp,_SP(%a0)


|restauration de "new_context"
|%a0 repere new_context
move.l %d0,%a0
|CAS DE LA VM
|
move.l _U_SRP(%a0),%d0
movec.l %d0,%srp
movec.l %d0,%urp
pflusha
|
move.l _SP(%a0),%sp
movem.l (%sp)+,%d2-%d7/%a2-%a6
move.l _USP(%a0),%a1
move.l %a1,%usp

rts
