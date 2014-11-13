/* RP 25 / 01 / 1999
 *
 * simulation du context-switching sur unix avec setjmp/longjmp
 *
 *
 *
 * ERREUR FREQUENTE
 *	la pile descend
 *	sur ensibull, penser à utiliser setjump.o hacke par D. Veillard
 *	le n° de pid au retour d'un longjmp doit etre conservé en zone globale
 *
 */

#include <setjmp.h>
#include <stdio.h>

#define NBPROC	3
#define N	500

typedef void __f();
//pour les piles de chaque process
char SP[N*(NBPROC+1)];

//un Process Control Block simpliste
typedef struct pcb {
	//pour sauvegarder le contexte de chaque process
	jmp_buf BUFF;
	//pour repérer le code de chaque process
	__f *PROCESS;
} PCB;

PCB PROCS[NBPROC];

#define newpid(n) ((n+1) % NBPROC)
int pidactif;
int p;

void switch_ctxt(int o, int n)
{
if (!setjmp(PROCS[o].BUFF))
     longjmp(PROCS[n].BUFF,1);
}
void reschedule()
{
int old,new;
old = pidactif;
new = pidactif = newpid(pidactif);
switch_ctxt(old,new);
}

void process0()
{
int i,j,k,l,m,n,o;

i=1;j=2;k=3;l=4;m=5;n=6;o=7;
while(1) {
        printf("je suis p0 %d %d %d %d %d %d %d\n",i,j,k,l,m,n,o);
        sleep(1);
        reschedule();
        }
}

void process1()
{
while(1) {
        printf("JE SUIS P1\n");
        sleep(1);
        reschedule();
        }
}

void process2()
{
while(1) {
        printf("I am p2\n");
        sleep(1);
        reschedule();
        }
}
void create(void process(), int pid)
{
if (setjmp(PROCS[pid].BUFF)) {
   //retour depuis longjmp
   //démarrer l'exécution du code  
   PROCS[pidactif].PROCESS();
   //attention : le n° de pid doit etre conservé en zone globale!!!
   }
else {
   //après le setjmp
   //mise à jour pile
//#ifdef i386_linux
   PROCS[pid].BUFF->__sp = (__ptr_t)(SP + pid*N+N/2); //la pile descend !!!
   PROCS[pid].BUFF->__bp = (__ptr_t)(SP + pid*N+N);
//#endif
//#ifdef ensibull_aix
//   (PROCS[pid].BUFF)[3 à voir] = (unsigned int)(SP + pid*N+N); //la pile descend !!!
//#endif
   //et du code
   PROCS[pid].PROCESS=process;
   }
}

void resume(int pid)
{
pidactif=pid;
longjmp(PROCS[pid].BUFF,1);
}

void main()
{
create(process0,0);
create(process2,2);
create(process1,1);

resume(1);
}
