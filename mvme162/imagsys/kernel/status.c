/*
 *
 */
 
#include "process.h"
#include "semaphore.h"
#include "sleep.h"
#include "drivers.h"
#include "status.h"

#define nl 	strcat(result,"\012\015")
#define print(s) outstr(s,s+strlen(s))

int sys_status(int n,char *result)
{
int i, val;
char s[30];
PROCESS *p;

val=0;
*result=0;

switch(n) {
  
/*   case statusBUDDY : 
   		check_mem();
	     	break;
*/
   case statusSEMA : for(i=0;i<NSEMA;i++)
                 if(SEMA_ARRAY[i].s_status == VALID) {
                     strcat(result,"semaphore ");
                     ntos(i,s);
                     strcat(result,s);
		     strcat(result," ");
		     strcat(result,SEMA_ARRAY[i].s_name);
                     strcat(result," count ");
                     n_htos(SEMA_ARRAY[i].s_count,s);
                     strcat(result,s);
	     	     nl;
                     }
		 break;
   case statusSLEEP :
	      strcat(result,"sleeping");
	      nl;
              p = SLEEP;
              while(p != NULL) {
		   strcat(result,p->name);
		   strcat(result," ");
		   ntos(p->delay,s);
		   strcat(result,s);
	     	nl;
		   p = p->s_next;
   		   }
		   break;
		   
   case statusDESCR : 
	     	strcat(result,"file descriptor : slot/device");
	     	nl;
		for(i = 0;i<NFD;i++)
		   if (fdtab[i].libre == FDUSED) {
			strcat(result,"file desc. ");
			ntos(i,s);
			strcat(result,s);
			strcat(result,"/");
			ntos(fdtab[i].device, s);
			strcat(result,s);
	     		nl;
			}
	      	strcat(result,"device descriptor : devnum major/minor idata/odata");
	     	nl;
		for(i = 0;i<NDEV;i++) {
			strcat(result,"devnum ");
			ntos(i,s);
			strcat(result,s);
			strcat(result," ");
			ntos(devtab[i].major,s);
			strcat(result,s);
			strcat(result,"/");
			ntos(devtab[i].minor,s);
			strcat(result,s);
			strcat(result," ");
			ntos(tty[devtab[i].minor].idata,s);
			strcat(result,s);
			strcat(result,"/");
			ntos(tty[devtab[i].minor].odata,s);
			strcat(result,s);
	     nl;
			}
              break;
   case statusPROCS : 
		for(i=0;i<NBPROC;i++) 
		   if (PROCS_ARRAY[i].status != INEXISTANT) {
                      ntos(i,s);
		      strcat(result,s);
		      strcat(result," : ");
		      strcat(result,PROCS_ARRAY[i].name);
                      strcat(result," priorité ");
                      ntos(PROCS_ARRAY[i].prio,s);
                      strcat(result,s);
                 switch(PROCS_ARRAY[i].status) {
                       case SUSPENDU : 	strcat(result," suspendu");
                                       	break;
                       case SLEEPING : 	strcat(result," sleeping");
                                       	break;
                       case ACTIVABLE : strcat(result," activable");
                                        break;
                       case INEXISTANT : strcat(result," inexistant");
                                        break;
                       case WAITING : 	strcat(result," waiting sur ");
					strcat(result,((SEMAPHORE*) PROCS_ARRAY[i].sem)->s_name);
                                        break;
		       }
 	     	nl;
               }
                break;

   default : strcat(result,"choix non valide");
	     nl;
   }
return 0;
}
