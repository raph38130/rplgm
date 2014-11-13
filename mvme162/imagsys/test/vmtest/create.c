/*
 * $Id: create.c,v 1.1 1998/04/08 22:13:52 raph Exp $
 *
 * SSP initiale
 *

D2		---
D3		   |
D4		   |
D5		   |
D6		   |
D7		   |	movem de context-switch.s
A2		   |
A3		   |
A4		   |
A5		   |
A6		---

PC = adresse go		rts de context-switch.s

D0		---
D1		   |
A0		   |	movem de go
A1		---

SR = 0 pour faire passer en mode U   ---
PC = adresse du code process		| rte de go
FORMAT				     ---

 *
 */

#include "mem.h"
#include "process.h"
#include "sys.h"
#include "register.h"



#define Max(X,Y) ((X)>(Y)?(X):(Y))
#define print(s) outstr(s,s+strlen(s))

//appel à sys_kill
//en fait, "end()" devrait faire appell au kill défini dans lib/func.c
//pour respecter le plan mémoire
int kill(int pid)
{
SYSCALL(KILL);
return pid;
}

//lorsqu'un process U termine, cela se fait par un appel à end()
void end()
{
//le processus appelant est en mode U, il faut donc utiliser kill() (pas sys_kill)
//sys_getpid() possible par contre
kill(sys_getpid());
}
//pour démarrer un processus en mode U (à la suite de "sys_create")
void go()
{
asm(".globl start
start:
   movem.l (%sp)+,%d0-%d1/%a0-%a1
   rte");
}

int new_pid()
{
int i;
for (i=0;i<NBPROC;i++)
	if (PROCS_ARRAY[i].status == INEXISTANT) break;
if (i >= NBPROC) return -1;
if (i > PID) PID = i;
return i;
}

//version de create avec demande d'allocation memoire pour les piles
//on suppose le code en place
int sys_create(int (*ptfunc)(int),
           int ssize, 
           int prio, 
           char *name, 
           int nbargs, int *args)
{
PROCESS *p;
int s,page,offset;
void *addr;
char s1[20];

cli();
print("create");
//on recycle les numero de pid libérés par kill
if ((s=new_pid()) != -1) {
	p = &(PROCS_ARRAY[PID]);
	p->pid = s;
	}
else return -1;     

//il faut une priorité strictement superieure à 0
if ((prio > 0) && (prio <= 255))
    p->prio = prio; 
else return -1;
     
if (strlen(name) < taille_PROCESS_NAME) 
   strcpy(p->name,name);
else return -1;

//mise a jour des registres composant le mot d'état
//voir context-switch.s avant toute modification
#define SSP 	0	//SP mode S (kernel stack)
#define USP	1	//user stack
#define U_SRP	2	//URP/SRP

//DRAM Alloc 1 page, il faudrait Max(ssize,TAILLE)
//addr=DRAM_Alloc();
p->stack = (int)0xFE000000-4096;		// bas de la zone pile

//p->code = (int) ptfunc;
page = (int) ptfunc & 0xFFFFF000;
offset = (int) ptfunc % 0x1000;
p->code = 0x80000000+offset;
n_htos(page,s1);
print(s1);
n_htos(offset,s1);
print(s1);

//construire Arbres
p->context[U_SRP] = (int)AllocTablePages(NIV1_2);

print("a");
//code e2s 2 pages + 162 Bug (0 --- 0x10000)
MapEspaceVirtuel(p->context[U_SRP],0x00000,0x10000+2*4096-1,TRANSPARANT|MODE_S,0);//modeS
//ssp e2s 1 page
MapEspaceVirtuel(p->context[U_SRP],0x400000-4096,0x3FFFFF,TRANSPARANT|MODE_S,0);  //modeS
//code process 1 page
MapEspaceVirtuel(p->context[U_SRP],0x80000000,0x80000000+4095,0,page);
//pile process 1 page
MapEspaceVirtuel(p->context[U_SRP],0xFE000000-4096,0xFE000000-1,0,DRAM_Alloc());
print("b");
asm("move.l %0,%%a0;movec.l %%a0,%%srp" : : "a" (p->context[U_SRP]) : "a0"  );
asm("  
    |vm Enable, page de 4k
    move.w #0x8000,%d0
    movec.l %d0,%tcr
    pflusha
    " );
print("c");
p->status = SUSPENDU;
p->suivant = NULL;

p->sem = NULL;
p->delay = 0;

//valeur initiale des piles
p->context[USP] = p->stack + PROCESS_STACK;
p->context[SSP]  = p->stack + K_STACK;

//construire le contenu de la pile USP
//en mettant sur USP l'adresse d'une fonction du genre kill(getpid())
//ainsi, un process qui termine le fait en appellant end()
//
//CAS DE LA VM
//pour faire ceci, il faut couvrir les piles...
StoreLong(p->context[USP], (int) end);

//construire le contenu de la pile SSP afin de pouvoir démarrer le process
//lors d'un scheduling
#define SavedByTrap		4 	//d0-d1/a0-a1
#define SavedBySwitch		11 	//d2-d7/a2-a6
//here : registers saved by switch

//adresse de :	movem.l (%sp)+,registersSavedByTrap
//		rte
StoreLong(p->context[SSP]+ SavedBySwitch*4, (int)go + 4); 
//here : registers saved by trap

//si coprocesseur mathématique, autres sauvegardes...

StoreWord(p->context[SSP]+(SavedBySwitch+SavedByTrap)*4+4+0, 0);	//SR
StoreLong(p->context[SSP]+(SavedBySwitch+SavedByTrap)*4+4+2, p->code);	//PC
StoreWord(p->context[SSP]+(SavedBySwitch+SavedByTrap)*4+4+6, 0);	//FORMAT

return p->pid;
}

