/*
 * process.c
 * $Id$
 * divers process utilisateurs pour le test des primitives du noyau
 */


#define print(s) outstr(s,s+strlen(s))



int process2()
{
char s[20];
int i,j=0;
while(j++<20) {
   print("PROCESS2");
   for(i=0;i<1000000;i++);
   //si ligne suivante, alors faute de page au 6ème passage 6*800=4800>4096)
   //i=*((int*)(0x80000000+j*800));ntos(i,s);print(s);
   //si ligne suivante et protection kernel alors faute pour j=10
   if (j==100) {
      print("acces kernel");
      i=*((int*)(0x10000));ntos(i,s);print(s);
      }
   }

return 0;
}

