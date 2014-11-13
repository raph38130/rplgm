/*
 * $Id: primitives.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 * 24 / 12 / 97 fonctions élémentaires
 *              manque sleep() (liste de delta time)
 * 24 / 02 / 98 fait
 * A FAIRE les primitives système doivent etre accessibles par trap
 *	   pour pouvoir etre sortie de l'espace mémoire utilisateur
 *         fait
 * 
 *         faire un fichier par fonction
 *		FAIT 20 / 02 / 98
 */

#include "process.h"
#include "register.h"

#define print(s) outstr(s,s+strlen(s))


//tableaux des process de E2S
PROCESS PROCS_ARRAY[NBPROC];

//pour démarrer
PROCESS DUMMY;

//prochain pid a créer
int PID=0;	//entre 0 et NBPROC

//liste des process actifs :
//c'est à dire de plus haute priorité et dans l'état ACTIVABLE
//liste circulaire doublement chainée avant/arrière
PROCESS *READY = NULL;

//le processes actif de E2S
PROCESS *ACTIF = NULL;



/////////////////////////////////////////
/*
 * sort p de la liste READY des process éligibles 
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
 * generer la liste READY des process éligibles
 */
void genere()
{
PROCESS *p,*last;
int i,highest;

READY = &(PROCS_ARRAY[0]);
READY->suivant = READY;
highest = READY->prio;
for(i=1;i<=PID;i++) {
   p = &(PROCS_ARRAY[i]);
   if (p->status == ACTIVABLE) {
      if (p->prio > highest) {
         highest = p->prio;
         p->suivant = p;
         READY = last = p;
	 //print("genere cas > ");
         }
      else if (p->prio == highest) {
              p->suivant = READY;
              last->suivant = READY = p;
	      //print("genere cas == ");
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

//la priorité d'un processus Activable change	
if (newprio > actualprio) {
         p->suivant = p;
         READY = p;
         reschedule();
         }
else {
     if (newprio == actualprio) {
              p->suivant = READY->suivant;
              READY->suivant = p;
              READY = p;
              reschedule();
              }
      else {//dans ce cas newprio < actualprio
                   //il faut sortir p de READY
		   if (READY->suivant == READY)
                        genere();
		   else 
                       enleve(p);
                   reschedule();
            }
      }
return oldprio;
}


