#include "commun.h"
int SP_exception;

void Exception()
{
char * stackp;
long int *l_p;
short int *s_p;
short int si;
long int li;
int CP, CU, CT, CM ;   /*   flags du Special Status Word   */
int TT, TM;
int mmusr;

stackp = (char *) SP_exception;
kprintf("*** Exception SP = %08x\n",SP_exception);
s_p = (short int *) stackp;
l_p = (long int *) (stackp + 2);
kprintf("   SR = %04x PC = %08x\n",*s_p, *l_p);

s_p = (short int *) (stackp + 6);
si = *s_p;
kprintf("   Format = %d Vector Offset = %4x\n", si >> 12, si &0xfff);


/*   le Special Status Word   */
/*   ----------------------   */
s_p = (short int *) (stackp + 12);
si = *s_p;
CP = 0; CU = 0; CT = 0; CM = 0;
kprintf("   Special Status word = %04x",si);
if (si & 0x8000) { CP = 1; kprintf(" CP");}
if (si & 0x4000) { CU = 1; kprintf(" CU");}
if (si & 0x2000) { CT = 1; kprintf(" CT");}
if (si & 0x1000) { CM = 1; kprintf(" CM");}
if (si & 0x0800) kprintf(" MA");
if (si & 0x0400) kprintf(" ATC");
if (si & 0x0200) kprintf(" LK");
if (si & 0x0100) kprintf(" R"); else kprintf(" W");
kprintf(" Size = %d",(si & 0x6) >> 5);
TT = (si & 0x18) >> 3;
kprintf(" TT = ");
switch(TT)
   {
   case 0 : kprintf("Normal Access"); break;
   case 1 : kprintf("MOVE16"); break;
   case 2 : kprintf("Alternate LFC"); break;
   case 3 : kprintf("Acknowledge"); break;
   }

TM = si &0x7;
switch(TM)
   {
   case 0 : kprintf(" TM = Data Cache Push\n"); break;
   case 1 : kprintf(" TM = User Data\n"); break;
   case 2 : kprintf(" TM = User Code\n"); break;
   case 3 : kprintf(" TM = MMU Data\n"); break;
   case 4 : kprintf(" TM = MMU Code\n"); break;
   case 5 : kprintf(" TM = System Data\n"); break;
   case 6 : kprintf(" TM = System Code\n"); break;
   case 7 : kprintf(" TM = ???\n"); break;
   }

if (CP || CU || CT || CM) 
   {
   l_p = (long int *) (stackp + 8);
   kprintf("   Effective Address = %08x\n",*l_p);
   }
else kprintf("   Effective Address non significatif\n");

l_p = (long int *) (stackp + 20);
kprintf("   Fault Address = %08x\n",*l_p);
mmusr = testadr(*l_p,TM);
kprintf("   MMUSR apres PTEST : %4x\n",mmusr);


ret16xbug();
}
