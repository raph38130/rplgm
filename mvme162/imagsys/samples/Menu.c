/*
 * $Id$
 *
 *
 */

#include "process.h"
#include "kernel.h"
#include "semaphore.h"
#include "sysfunc.h"
#include "sleep.h"
#include "drivers.h"
#include "mem.h"
#include "status.h"


void check_mem()
{
int f1;
char s[30];
int L[log2heapsize+1],
    largest,
    size,
    i;
f1 = open("tty1","w");
size = getfreemem(L,&largest);
write(f1,"memoire disponible ",0);
ntos(size,s);
write(f1,s,0);
write(f1," plus gros bloc ",0);
ntos(largest,s);
writeln(f1,s);

for(i=0;i<=log2heapsize;i++) {
   write(f1,"taille 2 ^ ",0);
   ntos(i,s);
   write(f1,s,0);
   write(f1," nombre ",0);
   ntos(L[i],s);
   writeln(f1,s);
   }
//close(f1);
}

void start()
{
char s[30],buffer[150];
int f1,f2,i,n,addr;

f1 = open("tty1","w");
f2 = open("tty1","r");
while(1) {
   read(f2,s,0);
   switch(s[0]) {
   case '?' :    
   		writeln(f1,"     (r)esume");
 		writeln(f1,"     (s)uspend");  
		writeln(f1,"     (k)ill process");
		writeln(f1,"     (c)hprio");
		writeln(f1,"     (g)etprio");
		writeln(f1,"     etat sema (1)");
 		writeln(f1,"          sleep(2)");
		writeln(f1,"          fdtab(3)");
		writeln(f1,"          procs(4)"); 
		//   writeln(f1,"     (d)el sema.");
		//   writeln(f1,"     (a)llocate mem.");
		//   writeln(f1,"     (f)ree mem.");
		//   writeln(f1,"     (n)ew procs"); 
		writeln(f1,"     (l)oad"); 
		break;
   /*
   case 'a' :
   case 'A' :writeln(f1,"taille desiree :");
   	     read(f2,s,0);
	     n = ston(s);
	     addr = (int) malloc(n);
   	     ntos(addr,s);
	     write(f1,"adresse obtenue ",0);
   	     writeln(f1,s);
	     check_mem();
	     break;
   case 'f' :
   case 'F' :writeln(f1,"adresse à libérer :");
   	     read(f2,s,0);
	     addr = ston(s);
	     free(addr);
	     check_mem();
   	     break;
*/

   case 'r' :
   case 'R' :
              write(f1,"numero de pid : ",0);
              read(f2,s,0);
              ntos(resume(ston(s)),s);
              write(f1,s,0);
	      
              break;
   case 'c' :
   case 'C' :
              write(f1,"numero de pid : ",0);
              read(f2,s,0);
              n = ston(s);
              write(f1,"new prio : ",0);
              read(f2,s,0);
              chprio(n,ston(s));
              break;
   /*
   case 'n' :
   case 'N' : 
              write(f1,"adresse : ",0);
              read(f2,s,0);
              //sscanf(s,"%d",&n);
              n=ston(s);
	      write(f1,"prio : ",0);
              read(f2,s,0);
	      //siprintf(s,"adresse lue %d prio %s\n",n,s);
              create((int (*)())n,0,ston(s),"new procs",0,0);
	      //si la ligne suivante est en commentaire, le resume() provoque un
	      //acces fault
	      //print("apres create");
              break;
	      */
   case 'l' :
   case 'L' :
              write(f1,"nom : ",0);
              read(f2,s,0);
              load(s);
              break;
	      
   case 'k' :
   case 'K' :
              write(f1,"numero de pid : ",0);
              read(f2,s,0);
              ntos(kill(ston(s)),s);
              write(f1,s,0);
              break;
   case 's' :
   case 'S' :
              write(f1,"numero de pid : ",0);
              read(f2,s,0);
              ntos(suspend(ston(s)),s);
              write(f1,s,0);
              break;
   case 'g' :
   case 'G' : write(f1,"numero de pid : ",0);
              read(f2,s,0);
              write(f1,"priorité = ",0);
	      ntos(getprio(ston(s)),s);
              writeln(f1,s);
              break;
/*   case 'd' :
   case 'D' :
              write(f1,"numero de sema : ",0);
              read(f2,s,0);
              if (sdelete(&(SEMA_ARRAY[ston(s)])) == -1) 
                  writeln(f1,"sema non vide");
              break;
*/   

   case '1' : status(statusSEMA,buffer);
   	      write(f1,buffer,0);
              break;
   case '2' : status(statusSLEEP,buffer);
   	      write(f1,buffer,0);
              break;
   case '3' : status(statusDESCR,buffer);
   	      write(f1,buffer,0);
              break;
   case '4' : status(statusPROCS,buffer);
   	      write(f1,buffer,0);                
              break;
   default : writeln(f1,"choix non valide");
       }
   }
}
