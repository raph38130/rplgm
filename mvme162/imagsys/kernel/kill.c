/*
 * $Id: kill.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 */

#include "process.h"
#include "semaphore.h"
#include "register.h"
#include "mem.h"
#include "sleep.h"

int sys_kill(int pid)
{
PROCESS *p,*pred;
SEMAPHORE *sema;

cli();
if (badpid(pid)) return -1;
p = &(PROCS_ARRAY[pid]); 
switch(p->status) {
   case ACTIVABLE : //un processus dans cet état ne fait partie d'aucune liste
   		    //de process en attente (sleeping ou waiting)
   		    p->status = INEXISTANT;
                    //liberer ressources mémoire
		    Buddy_Dealloc(&heap,(void*)p->stack);

		    //si le process tué est le process courant
                    if (ACTIF == p) {
			if (ACTIF->suivant == ACTIF) 
				genere();
			else 	enleve(p);
			reschedule();
 			}
		   break;
		   
   case SUSPENDU : p->status = INEXISTANT;
                   //liberer ressources mémoire
               	   Buddy_Dealloc(&heap,(void*)p->stack);
    		   break;
		   
   case WAITING  : //il faut enlever p de (p->sem)->s_list
		   sema = (SEMAPHORE *) p->sem;
		   pred = sema->s_list;
                   if (pred == p) sema->s_list = p->s_next;
                   else {
			while (pred->s_next != p) pred = pred->s_next;
			pred->s_next = p->s_next;
			}
		   p->status = INEXISTANT;
		   //mettre à jour le compteur du sémaphore
		   sema->s_count --;
                   //liberer ressources mémoire
                   Buddy_Dealloc(&heap,(void*)p->stack);
		   break;
   case SLEEPING : //il faut enlever p de la liste SLEEPING
   		   pred = SLEEP;
                   if (pred == p) SLEEP = p->s_next;
                   else {
			while (pred->s_next != p) pred = pred->s_next;
			pred->s_next = p->s_next;
			}
		   if (p->s_next != NULL) p->s_next->delay += p->delay;
		   p->status = INEXISTANT;
   		   break;
   default :       //process dans un état non tuable (car pas encore fait)
 		   
		   return -1;
   }
return 0;
}
