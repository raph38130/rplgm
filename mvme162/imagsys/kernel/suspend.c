/*
 * $Id: suspend.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 */

#include "process.h"
#include "register.h"

//on ne peut suspendre qu'un process qui est en état activable
int sys_suspend (int pid)
{
PROCESS *p;

cli();
if (badpid(pid)) return -1;
p = &(PROCS_ARRAY[pid]); 
if (p->status != ACTIVABLE) return -1;

p->status = SUSPENDU;
if (p->prio == ACTIF->prio)
   if (READY->suivant == READY)
       //un seul process dans READY, c'est celui qu'on suspend
       //il faut donc régénérer la liste
       genere();
   else //il suffit d'enlever p de READY
        //si p == ACTIF ce n'est pas un cas particulier
       enleve(p);
reschedule();
return pid;
}

