/*
 * simulation de noyau avec unix : setjmp/longjmp
 */
#include <setjmp.h>

#define NBPROC 	2
#define N	100
//on se limite à NBPROC=2 process pour cette démo simpliste
#define next_pid(n) (n+1) % NBPROC
//simulation sur 80x86
int SP[NBPROC][N];
int BP[NBPROC][N];
jmp_buf ContextBuf[NBPROC];
int pid_actif;

int switch_ctxt(int pid1,int pid2)
{
if(!setjmp(ContextBuf[pid1])) longjmp(ContextBuf[pid2],1);
}
void reschedule()
{
int old,new;

old = pid_actif;
new = pid_actif = next_pid(pid_actif);
switch_ctxt(old,new);
}

void process1()
{

while(1){
	printf("je suis le process1\n");
	sleep(1);
	reschedule();
	}
}

void process2()
{
while(1){
	printf("JE SUIS LE PROCESS2\n");
	sleep(1);
	reschedule();
	}
}
void create(void process(), int pid)
{
ContextBuf[pid]->__pc = (__ptr_t) process;
ContextBuf[pid]->__sp = (__ptr_t) (SP[pid]+N/2);
ContextBuf[pid]->__bp = (__ptr_t) (BP[pid]+N/2);
}


int main()
{
//init
pid_actif = 0;
create(process1,0);
create(process2,1);

//GO !
process1();
return 0;
}
