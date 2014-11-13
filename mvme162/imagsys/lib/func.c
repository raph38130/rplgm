
/* projet système 1997-1998
 * RP
 * $Id$
 *
 * 
 */

#include "sys.h"
#include "register.h"
#include "process.h"
#include "semaphore.h"

/////////////////// I . O . //////////////////
int open(char *file, char *mode)
{
SYSCALL(OPEN);
return (int) file;
}
int close(int f)
{
SYSCALL(CLOSE);
return f;
}
int read(int f, char *buffer , int n)
{
SYSCALL(READ);
return f;
}
int write(int f, char *buffer , int n)
{
SYSCALL(WRITE);
return f;
}
int ioctl(int f, int mode)
{
SYSCALL(IOCTL);
return f;
}

/////////////////// SEMAPHORE /////////////////
SEMAPHORE* screate(int value, char *name)
{
SYSCALL(SCREATE);
return (SEMAPHORE*) value;
}
int sdelete(SEMAPHORE *sema)
{
SYSCALL(SDELETE);
return (int) sema;
}
int signal(SEMAPHORE *sema)
{
SYSCALL(SIGNAL);
return (int) sema;
}
int wait(SEMAPHORE *sema)
{
SYSCALL(WAIT);
return (int) sema;
}


////////////////PROCESS MGMT /////////////////
int kill(int pid)
{
SYSCALL(KILL);
return pid;
}

int resume(int pid)
{
SYSCALL(RESUME);
return pid;
}

int create(int (*f)(int),int ssize, int prio, char *name, int nbargs, int *args)
{
SYSCALL(CREATE);
return (int) f; //pid en fait
}

int getprio(int pid)
{
SYSCALL(GETPRIO);
return pid;
}

int _getpid(int pid)
{
SYSCALL(GETPID);
return pid;
}

int chprio(int pid, int prio)
{
SYSCALL(CHPRIO);
return pid; 
}

int suspend(int pid)
{
SYSCALL(SUSPEND);
return pid;
}

int status(int func,char*buffer)
{
SYSCALL(STATUS);
return func;
}

// TIME
int sleep(int pid)
{
SYSCALL(SLEEP);
return pid;
}

//memory managment
void* malloc(int size)
{
SYSCALL(MALLOC);
return (void*)size;
}
int free(void* addr)
{
SYSCALL(MFREE);
return (int)addr;
}
int getfreemem(int*liste,int*largest)
{
SYSCALL(GETFREEMEM);
return (int) liste;
}

int load(char*nom)
{
SYSCALL(LOAD);
return (int) nom;
}


//pour terminer proprement un processu
//insérer par crt0
void end()
{
kill(_getpid(0));
}
