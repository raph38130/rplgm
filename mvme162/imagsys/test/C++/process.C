/* RP 25 / 01 / 1999
 *
 * simulation du context-switching sur unix avec setjmp/longjmp
 *
 * utilisation de C++
 *
 *
 */

#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>
#include <stream.h>

#define NBPROC	3
#define N	900	//N=800 dump core
#define newpid(n) ((n+1) % NBPROC)

int pidactif;

typedef void __f();
//pour les piles de chaque process
char SP[N*(NBPROC+1)];

//un Process Control Block simpliste
class PCB {
private:
	//pour sauvegarder le contexte de chaque process
	jmp_buf BUFF;
	//pour repérer le code de chaque process
	__f *PROCESS;
public:
	int process_id;
	PCB(void (*)(), int );
	void PCB::resume();
} ;

jmp_buf *T[NBPROC];


void switch_ctxt(int o, int n)
{
if (!setjmp(*T[o]))
     	longjmp(*T[n],1);
}

void reschedule()
{
int old,next;
old = pidactif;
next = pidactif = newpid(pidactif);
switch_ctxt(old,next);
}

void process0()
{
int i,j,k,l,m,n,o;

i=1;j=2;k=3;l=4;m=5;n=6;o=7;
while(1) {
        cout << "je suis p0 " << i << " " << j << " " << k << " " << l << " " << m << " " << n << " " << o << "\n";
        //sleep(1);
        reschedule();
        }
}

void process1()
{
while(1) {
        cout << "JE SUIS P1" << "\n";
        //sleep(1);
        reschedule();
        }
}

void process2()
{
while(1) {
        cout << "i am p2" << "\n";
        //sleep(1);
        reschedule();
        }
}

PCB::PCB(void process(), int pid)
{
if (setjmp(BUFF)) {
   //retour depuis longjmp
   //démarrer l'exécution du code 
   PROCESS();
   //attention : le n° de pid doit etre conservé en zone globale!!!
   }
else {
   //après le setjmp
   T[pid]=&BUFF;
   process_id=pid;
   //mise à jour pile
   BUFF->__sp = (__ptr_t)(SP + pid*N+N/2); //la pile descend !!!
   BUFF->__bp = (__ptr_t)(SP + pid*N+N);
   //et du code
   PROCESS=process;
   }
}

void PCB::resume()
{
pidactif=process_id;
longjmp(BUFF,1);
}

void main()
{
PCB *x;

PCB(process0,0);
PCB(process2,2);
x=&PCB(process1,1);
getchar();
x->resume();
}
