/*
 * $Id: schedule.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 * 
 * le changement de contexte, fonction context-switch(), peut �tre initi� de deux fa�ons
 *	- par une IT du timer 1 		schedule()
 *      - par le systeme 			reschedule() 
 * ces deux fonctions ne diff�rent que parce que schedule() "acquite" l'it
 * et s'occupe des process "sleeping"
 *
 * a tout moment :
 * 	ACTIF est le process courant
 *	READY la liste des process �ligibles
 *      c'est le premier de cette liste qui est lanc� par (re)schedule
 */

#include "process.h"
#include "vme162.h"
#include "register.h"
#define print(s) outstr(s,s+strlen(s))

extern void switch_context(int *, int *);

//ex�cut� lors d'une it horloge
void schedule()
{
int *old, *new;

cli();
//acquiter le tick
//print("tick");
ack_tick();

//s'occuper des "sleeping process"
wake();

if (ACTIF == READY) 
   return ;

old = ACTIF->context;
new = READY->context;
ACTIF = READY;
READY=READY->suivant;

switch_context(old,new);
}

//g�n�r� par toute mise en attente d'un process
void reschedule()
{
int *old, *new;

old = ACTIF->context;
new = READY->context;
ACTIF = READY;
READY=READY->suivant;
switch_context(old,new);
}
