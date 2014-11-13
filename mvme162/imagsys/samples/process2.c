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




void start()
{
int pid1,pid2;

load("processX");
load("processY");
resume(3);
resume(4);

sleep(2000);
kill(3);
sleep(500);
kill(4);

}
