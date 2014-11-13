/*
 * $Id$
 */

#include "userprocess.h"


void init_mode_U()
{
int pid, pid1;

pid = create(process1,
            0,
            110,
            "process  1",
            0,(int*) 0);
pid = create(process2,
            0,
            110,
            "PROCESS  2",
            0,(int*) 0);
pid = create(process3,
            0,
            110,
            "PROCESS  3",
            0,(int*) 0);
pid = create(menu,
	    0,
	    110,
	    "shell    4",
	    0,(int*) 0);
resume(pid);	//lance le shell
pid = create(processA,
            0,
            110,
            "processA 5",
            0,(int*) 0);
pid = create(processX,
            0,
            110,
            "processX 6",
            0,(int*) 0);
pid = create(processY,
            0,
            110,
            "processY 7",
            0,(int*) 0);
pid = create(processZ,
            0,
            100,
            "processZ 8",
            0,(int*) 0);
pid = create(processW,
            0,
            110,
            "processW 9",
            0,(int*) 0);
}
