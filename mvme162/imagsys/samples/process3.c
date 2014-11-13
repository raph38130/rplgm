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
int i,f,f2;

f = open("tty0","w");

while(1) {
   writeln(f,"JE ");
   sleep(100);
   writeln(f,"SUIS ");
   sleep(100);
   writeln(f,"LE ");
   sleep(100);
   writeln(f,"PROCESS 3");
   sleep(100);
   }
close(f);

return 0;
}
