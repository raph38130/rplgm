/*
 * $Id: semaphore.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 * sémaphores  inspiré des "évènements" OS9
 * s->s_count = 0 libre
 * s->s_count > 0 occupé   wait insere si necessaire en tete de s_list
 *                       signal réveille en queue de s_list
 * s->s_count jamais négatif
 */
#define print(s) outstr(s,s+strlen(s))

#include "process.h"
#include "semaphore.h"
#include "register.h"



SEMAPHORE SEMA_ARRAY[NSEMA];

void init_sema()
{
int i;

for (i=0; i<NSEMA; i++)
    SEMA_ARRAY[i].s_status = FREE;
}

SEMAPHORE* sys_screate(int value, char *name)
{
int i;
SEMAPHORE *sema;

cli();
//chercher un semaphore disponible
i=0;
while ( (SEMA_ARRAY[i].s_status == VALID) && (i < NSEMA) ) i++;
if (i >= NSEMA) return (SEMAPHORE*) -1;
sema = &(SEMA_ARRAY[i]);
sema->s_status = VALID;
sema->s_count = value;
sema->s_list = NULL;
strncpy(sema->s_name, name,Taille_Semaphore_Name);
return sema;
}

//on ne peut pas supprimer un sémaphore non vide
int sys_sdelete(SEMAPHORE *sema)
{

cli();
if (isbadsema(sema)) return -1;
if (sema->s_list != NULL) return -1;
sema->s_status = FREE;
return 0;
}



int sys_wait(SEMAPHORE *sema)
{
PROCESS *p;

cli();
if (isbadsema(sema)) return -1;
sema->s_count++;
if (sema->s_count == 1) return 1;
p = ACTIF;
p->status = WAITING;
p->sem = sema;
p->s_next = sema->s_list;
sema->s_list = p;
//sortir p de READY
if (READY->suivant == READY)
   genere();
else 
   enleve(p);
reschedule();
return sema->s_count;
}

int sys_signal(SEMAPHORE *sema)
{
PROCESS *p, *pred;

cli();
if (isbadsema(sema)) return -1;
sema->s_count--;
//ne devient jamais négatif
if (sema->s_count<0) sema->s_count=0;

//reveiller un process (le plus ancien en attente, s'il existe)
if (sema->s_count > 0) {
    p = sema->s_list;
    pred = p;
    while (p->s_next != NULL) {
           pred = p;
           p = p->s_next;
           }
    //le supprimer de la s_list
    if (pred != p) 
       pred->s_next = NULL;
    else sema->s_list = NULL;
    p->status = SUSPENDU;
    sys_resume(p->pid);
    }
return sema->s_count;
}
