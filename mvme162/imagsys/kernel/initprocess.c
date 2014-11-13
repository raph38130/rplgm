/*
 * initprocess.c
 * $Id: initprocess.c,v 1.1 1998/04/08 22:15:09 raph Exp $
 * initialisation de E2S : creation de la liste initiale des process
 */

#include "process.h"
#include "kernel.h"
#include "vme162.h"
#include "io_call.h"
#include "semaphore.h"
#include "sleep.h"
#include "mem.h"
#include "sys_call.h"
#include "drivers.h"
#include "load.h"

void init_mode_S()
{
int i, pid0;
PROCESS *p;
int *TABLE;


//initialisation pour les semaphores
init_sema();
//initialisation pour sleep
init_sleeping();
//initialisation "file descriptor table"
init_fdtab();
//initialisation allocation mémoire
init_mm();
//initialisation mémoire virtuelle
init_mmu();
//initialisation load
init_load();
//initialisation PROCS_ARRAY
for (i=0;i<NBPROC;i++)
    PROCS_ARRAY[i].status = INEXISTANT;
    
//il faut un process de priorité 0
//pour consommer le temps CPU en l'abscence de process utilisateur
//c'est en fait le code a la fin de main() dans kernel
PID = 0;

pid0 = sys_create((int (*)()) 0, 
            0,
            1,	//priorité temporaire 
            "process  0",
            0,(int*) 0);

p=&(PROCS_ARRAY[0]);
p->status = ACTIVABLE;
//afin que "idle" ait la plus basse priorité possible, 
//on contourne la protection de sys_create
p->prio = 0;

ACTIF = READY = p->suivant = p;

/*pid0 = sys_create((int (*)())0x15004 , 
            0,
            100,	 
            "shell  1",
            0,(int*) 0);
*/
/*p=&(PROCS_ARRAY[1]);
p->status = ACTIVABLE;
genere();
*/
sys_load("init");
sys_resume(1);
//mmu_start();
//asm("move.l %0,%%a0;movec.l %%a0,%%srp" : : "a" (p->context[U_SRP]) : "a0"  );
//asm("move.l %0,%%a0;movec.l %%a0,%%urp" : : "a" (p->context[U_SRP]) : "a0"  );

//maintenant on peut scheduler
}
