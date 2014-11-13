/*
 * process.c
 * $Id$
 * divers process utilisateurs pour le test des primitives du noyau
 */


#define print(s) outstr(s,s+strlen(s))
int process1()
{
int i,j=0;
char s[20];

while(1) {
   print("process1 ");
   ntos(j,s);
   j++;
   print(s);
   for(i=0;i<1000000;i++);
   }
return 0;
}




