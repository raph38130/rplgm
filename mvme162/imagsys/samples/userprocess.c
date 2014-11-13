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


SEMAPHORE *sema1, *sema2;

int bcdton(int n)
{
return 10*(n>>4)+(n & 0xF);
}
//c'est du BCD !!!
void getdate(char *date)
{
char s,m,h,d,D,M,Y;
char _s[3],_m[3],_h[3],_d[3],_D[3],_M[3],_Y[3];

date[0]=0;

ReadByte(SECONDS,s);
ReadByte(MINUTES,m);
ReadByte(HOUR,h);
ReadByte(DAY,d);
ReadByte(DATE,D);
ReadByte(MONTH,M);
ReadByte(YEAR,Y);
ntos(bcdton(s & 0x7F),_s);
ntos(bcdton(m & 0x7F),_m);
ntos(bcdton(h & 0x3F),_h);
ntos(bcdton(d & 0x07),_d);
ntos(bcdton(D & 0x3F),_D);
ntos(bcdton(M & 0x1F),_M);
ntos(bcdton(Y & 0xFF),_Y);
strcpy(date,_Y);
strcat(date," / ");
strcat(date,_M);
strcat(date," / ");
strcat(date,_D);
strcat(date,"   ");
strcat(date,_h);
strcat(date,":");
strcat(date,_m);
strcat(date,":");
strcat(date,_s);
}

int process1()
{
int i,f,f2;
char buff[80];


f=open("tty0","w");
f2=open("tty0","r");
while(1) {
   writeln(f,"entrer delai : ");
   read(f2,buff,0);
   i = ston(buff);
   getdate(buff);
   writeln(f,buff);
   sleep(i);
   getdate(buff);
   writeln(f,buff);
   }
close(f);
return 0;
}

int process2()
{
int i,f1,f2;
char *s1="JE SUIS LE PROCESS2\012\015";
char  s2[20];

f1=open("tty0","w");
f2=open("tty0","r");
ioctl(f2,RAW);
writeln(f1,s1);
write(f1,"entrer une chaine : ",0);
read(f2,s2,0);
writeln(f1,"");
writeln(f1,s2);
close(f1);
close(f2);
return 0;
}

int process3()
{
int i=0,f;
char *s1="0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 \012\015";
char *s2="abcdefghijklmnopqrstuvwxyz 0123456789\012\015";

f=open("tty1","w");
while(i++<10) {
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   write(f,s1,0);
   write(f,s2,0);
   sleep(100);
   }
close(f);
return 0;
}


//test des primitives resume() et suspend()
int processA()
{
char c[3];
int i,f1,f2;

f1=open("tty0","w");
f2=open("tty0","r");
while(1) {
writeln(f1,"taper R pour resume(1)");
writeln(f1,"taper S pour suspend(1)");
read(f2,c,0);
if (c[0] == 'R') resume(1);
if (c[0] == 'S') suspend(1);
}
close(f1);
close(f2);
return 0;
}

//test des sémaphores
int processX()
{
int i,f;

f=open("tty0","w");
sema1 = screate(0, "sema1");
while(1) {
   wait(sema1);
   writeln(f,"je ");
   sleep(100);
   writeln(f,"suis ");
   sleep(100);
   writeln(f,"le ");
   sleep(100);
   writeln(f,"process X ");
   sleep(100);
   signal(sema1);
   }
close(f);
return 0;
}
int processY()
{
int i,f;

f=open("tty0","w");
while(1) {
   wait(sema1);
   writeln(f,"JE ");
   sleep(100);
   writeln(f,"SUIS ");
   sleep(100);
   writeln(f,"LE ");
   sleep(100);
   writeln(f,"PROCESS Y ");
   sleep(100);
   signal(sema1);
   }
close(f);
return 0;
}
int processZ()
{
int i,f;

f=open("tty0","w");
while(1) {
   wait(sema1);
   writeln(f,"aA ");
   for (i=0; i<100000; i++) ;
   writeln(f,"AAAA ");
   for (i=0; i<100000; i++) ;
   writeln(f,"AA ");
   for (i=0; i<100000; i++) ;
   writeln(f,"AAAAAAA a ");
   for (i=0; i<100000; i++) ;
   signal(sema1);
   }
close(f);
return 0;
}

//ce test ne marche plus au dela d'une certaine vitesse d'arrivée des
//données
int processW()
{
char *p = (char *) 0x300000;
int f1,f2,n=30;
char buff[250];
char buff2[10];

f2=open("tty0","r");
ioctl(f2,RAW);
f1=open("tty0","w");
while(1) {
	read(f2,buff,0);
	//strcat(p,buff);
	writeln(f1,buff);
	}
return 0;
}

void start(){
process3();
}
