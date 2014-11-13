/*
 * $Id: resume.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 */

#include "process.h"
#include "register.h"


int sys_resume(int pid)
{
PROCESS *p;

//cli();
if (badpid(pid)) return -1;   
p = &(PROCS_ARRAY[pid]); 
if (p->status != SUSPENDU) return -1;
   
p->status = ACTIVABLE;

if (p->prio > ACTIF->prio) {
   p->suivant = p;
   READY = p;
   reschedule();
   }
else if (p->prio == ACTIF->prio) {
   genere();
   reschedule();
   }
//si p->prio < ACTIF->prio on ne change rien
return pid;
}
