/*
 * process.c
 * $Id$
 * divers process utilisateurs pour le test des primitives du noyau
 */

#include "register.h"
#include "process.h"
#include "semaphore.h"
#include "sysfunc.h"
#include "drivers.h"
#include "vme162.h"





int start()
{
int i,f;

f=open("tty0","w");
while(1) {
   //wait(sema1);
   writeln(f,"je ");
   sleep(100);
   writeln(f,"suis ");
   sleep(100);
   writeln(f,"le ");
   sleep(100);
   writeln(f,"process X ");
   sleep(100);
   //signal(sema1);
   }
close(f);
return 0;
}
