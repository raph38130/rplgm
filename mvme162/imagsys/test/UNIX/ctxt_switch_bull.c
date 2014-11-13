#include <setjmp.h>
#include <stdio.h>
#define NBPROC	2
#define N	100

typedef void __f();
jmp_buf BUFF[NBPROC],buff;
int SP[NBPROC][N];
int BP[NBPROC][N];
__f *PROCESS[NBPROC];

int pidactif;

#define newpid(n) ((n+1) % NBPROC)



void switch_ctxt(int o, int n)
{
if(!setjmp(BUFF[o])) {
   longjmp(BUFF[n],n+10);  //+10 car on ne peut pas transmettre un pid=0
   }
}
void reschedule()
{
int old,new;

old = pidactif;
new = pidactif = newpid(pidactif);
switch_ctxt(old,new);
}
void process1()
{
while(1) {
        printf("je suis p1\n");
        sleep(1);
        reschedule();
        }
}

int f(int x)
{
return x*10;
}

void process2()
{
int i=0;
while(1) {
        printf("JE SUIS P2 %d\n",f(i++));
        sleep(1);
        reschedule();
        }
}


void create(void process(), int pid)
{
int s;

if (s=setjmp(BUFF[pid]))
   PROCESS[s-10]();
BUFF[pid]->__sp = (__ptr_t) (SP[pid]+N/2);
BUFF[pid]->__bp = (__ptr_t) (BP[pid]+N/2);
PROCESS[pid]=process;
}


main()
{
pidactif=0;
create(process1,0);
create(process2,1);

longjmp();
}



