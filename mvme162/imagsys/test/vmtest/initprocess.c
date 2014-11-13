/*
 * initprocess.c
 * $Id: initprocess.c,v 1.1 1998/04/08 22:15:09 raph Exp $
 * initialisation de E2S : creation de la liste initiale des process
 */
#define print(s) outstr(s,s+strlen(s))
#include "process.h"
#include "register.h"
/*#include "kernel.h"
#include "vme162.h"
#include "io_call.h"
#include "semaphore.h"
#include "sleep.h"
#include "mem.h"
#include "drivers.h"
*/
extern int sys_create(int (*)(int),int,int,char*,int,int*);
extern int sys_resume(int);


void init_mode_S()
{
int i,	 pid0,pid;
PROCESS *p;
char s[30];

//initialisation PROCS_ARRAY
for (i=0;i<NBPROC;i++)
    PROCS_ARRAY[i].status = INEXISTANT;
    
//initialisation mémoire virtuelle
print("0");
init_mm();
print("1");

//il faut un process de priorité 0
//pour consommer le temps CPU en l'abscence de process utilisateur
//c'est en fait le code a la fin de main() dans kernel
PID = 0;
pid0 = sys_create((int (*)()) 0, 
            0,
            1,	//priorité temporaire 
            "process  0",
            0,(int*) 0);
print("2");
p=&(PROCS_ARRAY[0]);
p->status = ACTIVABLE;
//afin que "idle" ait la plus basse priorité possible, 
//on contourne la protection de sys_create
p->prio = 0;

ACTIF = READY = p->suivant = p;


#define process1 0x1500E
#define process2 0x1701A
pid = sys_create(process2,
            0,
            110,
            "process  1",
            0,(int*) 0);
print("3");
ntos(pid,s);
print(s);
p=&(PROCS_ARRAY[pid]);
p->status = ACTIVABLE; 
pid = sys_create(process1,
            0,
            110,
            "PROCESS  2",
            0,(int*) 0); 
print("4");
p=&(PROCS_ARRAY[pid]);
p->status = ACTIVABLE;
ntos(pid,s);
print(s);
genere();
print("init soft ok");
//on prépare pour process0
p=&(PROCS_ARRAY[0]);
print("GO !");
#define U_SRP	2
asm("move.l %0,%%a0;movec.l %%a0,%%srp" : : "a" (p->context[U_SRP]) : "a0"  );
asm("move.l %0,%%a0;movec.l %%a0,%%urp" : : "a" (p->context[U_SRP]) : "a0"  );

//maintenant on peut scheduler
sti();
}
