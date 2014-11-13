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
#include "mem.h"
#include "sysfunc.h"
#include "sys_call.h"


void sys_call_select()
{ 
int n,    	//n° de fonction
    x1,x2,x3,x4, x5,x6;
//parametres récupérés sur la pile USP
fetch_n_arg1(n,x1);
switch(n) {
   //Process managment
   case KILL : 
            x1 = sys_kill(x1);
            break;
   case RESUME :
            x1 = sys_resume(x1);
            break;
   case CREATE :
            fetch_arg_create(x2,x3,x4,x5,x6);
            x1 = sys_create((int(*)(int))x1,x2,x3,(char*)x4,x5,(int*)x6);
            break;
   case GETPRIO :
            x1 = sys_getprio(x1);
            break;
   case GETPID :
            x1 = sys_getpid(x1);
            break;
   case CHPRIO :
            fetch_arg2(x2);
            x1 = sys_chprio(x1,x2);
            break;
   case SUSPEND :
            x1 = sys_suspend(x1);
            break;
   case SLEEP :
            x1 = sys_sleep(x1);
            break;
   case STATUS :
   	    fetch_arg2(x2);
            x1 = sys_status(x1,(char*)x2);
            break;
	    
	    
   //Semaphores
   case SCREATE :
            fetch_arg2(x2);
	    x1 = (int) sys_screate(x1,(char*)x2);
            break;
   case SDELETE :
	    x1 = sys_sdelete((SEMAPHORE*)x1);
            break;
   case SIGNAL:
	    x1 = sys_signal((SEMAPHORE*)x1);
            break;
   case WAIT :
	    x1 = sys_wait((SEMAPHORE*)x1);
            break;
   //I.O.
   case OPEN :
            fetch_arg2(x2);
	    x1 = sys_open((char*)x1,(char*)x2);
	    break;
   case CLOSE :
	    x1 = sys_close(x1);
	    break;
   case READ :
            fetch_arg2_3(x2,x3);
	    x1 = sys_read(x1,(char*)x2,x3);
	    break;
   case WRITE :
            fetch_arg2_3(x2,x3);
	    x1 = sys_write(x1,(char*)x2,x3);
	    break;
   case IOCTL :
            fetch_arg2(x2);
	    x1 = sys_ioctl(x1,x2);
	    break;
   //memory managment
   case MALLOC :
   	    x1 = (int) Buddy_Alloc(&heap,x1,0);
	    break;
   case MFREE :
   	    x1 = Buddy_Dealloc(&heap,(void*)x1);
	    break;
   case GETFREEMEM :
            fetch_arg2(x2);
   	    x1 = Buddy_GetFreeMemInHeap(&heap, (int*)x1,(int*) x2);
	    break;
	    
   case LOAD :
   	    x1 = sys_load((char*)x1);
	    break;
   //
   default : 
            x1 = -1;
            break;
   }
store_arg1(x1);
}


