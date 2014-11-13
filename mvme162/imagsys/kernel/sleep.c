/*
 * $Id: sleep.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 * les process sleeping sont dans une liste ordonn�e selon la date � laquelle il doivent etre r�veill�s
 * les delta sont dans le champ "delay" de chaque process descriptor
 * lors de chaque tick "schedule()" appelle "wake()" qui d�cr�mente le premier delta,
 * si il est �gal � 0 le process est r�veill�
 */

#include "register.h"
#include "process.h"
#include "sleep.h"


//la liste des process SLEEPING
PROCESS *SLEEP=NULL;


void init_sleeping()
{
SLEEP = NULL;
}

void sys_sleep(int delay)
{
PROCESS *p,*pred;

cli();
if (delay <= 0) {
   return ;
   }
ACTIF->status = SLEEPING;
//mettre ACTIF dans la liste SLEEP
if (SLEEP == NULL) {
   SLEEP = ACTIF;
   ACTIF->s_next = NULL;
   }
else {
   //rechercher l'emplacement en d�cr�mentant delay au fur et � mesure
   pred = p = SLEEP;
   while ((p != NULL) && (delay > p->delay)) {
	delay -= p->delay;
	pred = p;
	p = p->s_next;
	}
      if (pred == p) SLEEP = ACTIF;
      else pred->s_next = ACTIF;
      ACTIF->s_next = p;
      if (p != NULL ) p->delay -= delay;
      }
ACTIF->delay = delay;
//on regenere la liste READY et on reschedule
genere();
reschedule();
return ;
}



//appell� par schedule()
void wake()
{
PROCESS *p;
int prio;

if ((SLEEP != NULL) && (--(SLEEP->delay) <= 0)) {
	//r�veiller le premier process de la liste SLEEP
	p = SLEEP;
	SLEEP = SLEEP->s_next;

	p->status = ACTIVABLE;
        prio = p->prio;
 	if (prio > ACTIF->prio) {
            p->suivant = p;
            READY = p;
            }
   	else 
        if (prio == ACTIF->prio) genere();
        }
}
