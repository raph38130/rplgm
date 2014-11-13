/*
 * le MCchip
 *
 * RP 05 / 1998
 *
 *
 * remarque : l'appel a outstr() c'est à dire SYSCALL .OUTSTR se fait avec it
 * masquées et VBR spécial donc une it survenant pendant outstr aura un
 * comportement prévu par 162Bug
 */

#define print(s) outstr(s,s+strlen(s))

#define ABORT_CR 	0xFFF42043
#define MC_CR 		0xFFF42002
#define MC_IVBR		0xFFF42003
#define T1_CR 		0xFFF42017
#define T1_ICR		0xFFF4201B
#define T1_DUREE	0xFFF42004

#define MIEN	2		
#define IEN	0x10
#define LEVEL	7
#define level	7
#define ICLR	8		
#define CEN	1		
#define COC	2	

int test;
char *pb;
int *plw;

void f()
{
asm volatile("	movem.l %d0-%d1/%a0-%a1,-(%sp)");
pb = (char*)ABORT_CR;
*pb = IEN|ICLR|level;

test=5;

asm volatile("	movem.l (%sp)+,%d0-%d1/%a0-%a1
		rte");
}
void g()
{
asm volatile("	movem.l %d0-%d1/%a0-%a1,-(%sp)");
pb = (char*)T1_ICR;
*pb = IEN|ICLR|LEVEL;

test=7;

asm volatile("	movem.l (%sp)+,%d0-%d1/%a0-%a1
		rte");
}

		
main()

{
//main init
pb = (char*)MC_CR;
*pb = MIEN;
pb = (char*)MC_IVBR;
*pb = 0x50; 	//comme 162Bug sinon pas de SYSCALL
plw = (int*) (VBR + 4*(((int)*(pb))+0xE));
*plw = (int)f+4;
plw = (int*) (VBR + 4*(((int)*(pb))+0x9));
*plw = (int)g+4;

//init ABORT
pb = (char*)ABORT_CR;
*pb = IEN|level;

//init T1
plw = (int*)T1_DUREE;
*plw = 1000000;	//1 seconde
pb = (char*)T1_ICR;
*pb = IEN|LEVEL;
pb = (char*)T1_CR;
*pb = CEN|COC;

asm volatile("move.w #0x2000,%SR");

test=0;
while(1) 
	switch (test) {
		case 7 : print("ABORT");
			 test=0; //surtout pas apres le switch !
		         break;
		case 5 : print("tick");
			 test=0; //surtout pas apres le switch !
		         break;
		}
}
