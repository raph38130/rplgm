/*
 * $Id: create.c,v 1.1 1998/04/08 22:13:52 raph Exp $
 *
 *
 */
#include "process.h"
#include "sys.h"
#include "register.h"
#include "mem.h"

#define Max(X,Y) ((X)>(Y)?(X):(Y))

//MODIF : plus simple a implementer si c'est le crt0 qui fait l'appel à end()
//appel à sys_kill
//en fait, "end()" devrait faire appel au kill défini dans lib/func.c
//pour respecter le plan mémoire

/*
//lorsqu'un process U termine, cela se fait par un appel à end()
void end()
{
//le processus appelant est en mode U, il faut donc utiliser kill() (pas sys_kill)
//sys_getpid() possible par contre
kill(sys_getpid());
}*/
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

int sys_create(int (*ptfunc)(int),
           int ssize, 
           int prio, 
           char *name, 
           int nbargs, int *args)
{
PROCESS *p;
int s,srp;
void *addr;
int page,offset;
char s1[20];

cli();
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

//DRAM Alloc 1 page, il faudrait Max(ssize,TAILLE)
//addr=DRAM_Alloc();
p->stack = (int)0xFE000000-4096;		// bas de la zone pile

//p->code = (int) ptfunc;
//le point d'entrée dans le process n'est pas forcemment au début de la page
page = (int) ptfunc & 0xFFFFF000;
offset = (int) ptfunc % 0x1000;
p->code = 0x80000000+offset;

//construire Arbres
p->context[U_SRP] = (int)AllocTablePages(NIV1_2);

//code e2s 5 pages + 162 Bug (0 --- 0x10000)
MapEspaceVirtuel(p->context[U_SRP],
		(char*)0x00000,
		(char*)0x10000+5*4096-1,
		TRANSPARANT|MODE_S,0);
//ssp e2s 1 page
MapEspaceVirtuel(p->context[U_SRP],
		(char*)0x400000-4096,
		(char*)0x3FFFFF,
		TRANSPARANT|MODE_S,0);
//code process 1 page
//attention si pas transparant, une page a la fois !!!
MapEspaceVirtuel(p->context[U_SRP],
		(char*)0x80000000,
		(char*)0x80000000+4095,
		0,page);
/*MapEspaceVirtuel(p->context[U_SRP],
		(char*)0x80000000+4096,
		(char*)0x80000000+2*4096-1,
		0,page+4096); 
*/		
//pile process 1 page
MapEspaceVirtuel(p->context[U_SRP],
		(char*)0xFE000000-4096,
		(char*)0xFE000000-1,
		0,PageAlloc(DRAM));
//on installe l'arbre pour mettre à jour la pile
//sauvegarde de srp
asm("movec.l %%srp,%0" : "=a" (srp) : : "a0" );

asm("movec.l %0,%%srp" : : "a" (p->context[U_SRP]) : "a0"  );
asm("  
    |vm Enable, page de 4k
    move.w #0x8000,%d0
    movec.l %d0,%tcr
    pflusha
    " );

/*
if ((addr=Buddy_Alloc(&heap,Max(ssize,TAILLE),0)) != NULL)
   p->stack = (int)addr;		// bas de la zone pile
else return -1;

p->code = (int) ptfunc;
*/
p->status = SUSPENDU;
p->suivant = NULL;

p->sem = NULL;
p->delay = 0;

//valeur initiale des piles
p->context[USP] = p->stack + PROCESS_STACK;
p->context[SSP]  = p->stack + K_STACK;

/*
//construire le contenu de la pile USP
//en mettant sur USP l'adresse d'une fonction du genre kill(getpid())
//ainsi, un process qui termine le fait en appellant end()
StoreLong(p->context[USP], (int) end);
*/
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
//restauration de srp (sauf pour le pid 0)

if (p->pid!=0) asm("movec.l %0,%%srp;pflusha" : : "a" (srp) : "memory"  );
//print("created");
return p->pid;
}

/*
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
*/
