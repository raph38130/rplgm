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

int start()
{
int i,f,f2;
char buff[80];


f=open("tty1","w");
f2=open("tty1","r");
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

