/*
 * $Id: primitives.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 * 24 / 12 / 97 fonctions �l�mentaires
 *              manque sleep() (liste de delta time)
 * 24 / 02 / 98 fait
 * A FAIRE les primitives syst�me doivent etre accessibles par trap
 *	   pour pouvoir etre sortie de l'espace m�moire utilisateur
 *         fait
 * 
 *         faire un fichier par fonction
 *		FAIT 20 / 02 / 98
 */

#include "process.h"
#include "register.h"



//tableaux des process de E2S
PROCESS PROCS_ARRAY[NBPROC];

//pour d�marrer
PROCESS DUMMY;

//prochain pid a cr�er
int PID=0;	//entre 0 et NBPROC

//liste des process actifs :
//c'est � dire de plus haute priorit� et dans l'�tat ACTIVABLE
//liste circulaire doublement chain�e avant/arri�re
PROCESS *READY = NULL;

//le processes actif de E2S
PROCESS *ACTIF = NULL;



/////////////////////////////////////////
/*
 * sort p de la liste READY des process �ligibles
 *
 * int�ret : plus "rapide" que generer() 
 */
void enleve(PROCESS *p)
{
PROCESS *pred; 

pred = p;
while (pred->suivant != p) pred = pred->suivant;
if (p == READY) READY = READY->suivant;
else            pred->suivant = p->suivant;
p->suivant = NULL;
}
/*
 * generer la liste READY des process �ligibles
 */
void genere()
{
PROCESS *p,*last;
int i,highest;

READY = &(PROCS_ARRAY[0]);
highest = READY->prio;
for(i=0;i <= PID;i++) {
   p = &(PROCS_ARRAY[i]);
   if (p->status == ACTIVABLE) {
      if (p->prio > highest) {
         highest = p->prio;        
         last = READY = p->suivant = p;
         }
      else if (p->prio == highest) {
              p->suivant = READY;
              last->suivant = READY = p;
              }
      }
   }
}



int sys_getprio(int pid)
{
if (badpid(pid)) return -1;
return PROCS_ARRAY[pid].prio;
}

int sys_getpid()
{
return ACTIF->pid;
}

int sys_chprio(int pid, int newprio)
{
PROCESS *p;
int oldprio, actualprio;

cli();
if ((newprio <= 0) || (newprio > 255)) return -1;
if (badpid(pid)) return -1;
p = &(PROCS_ARRAY[pid]); 
if (p->prio == newprio) return newprio;
oldprio = p->prio;
actualprio = ACTIF->prio;
p->prio = newprio;
if (p->status != ACTIVABLE) return oldprio;

//la priorit� d'un processus Activable change	
if (newprio > actualprio) {
   p->suivant = p;
   READY = p;
   }
else //newprio <= actualprio
   genere();
reschedule();
return oldprio;
}


