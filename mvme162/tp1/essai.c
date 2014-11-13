/*
 * le MCchip
 * RP sam avr  9 10:11:09 UTC 2005
 *	- SetHandler
 * RP 05 / 1998
 *
 *
 * remarque : l'appel a outstr() c'est à dire SYSCALL .OUTSTR se fait avec it
 * masquées et VBR spécial donc une it survenant pendant outstr aura un
 * comportement prévu par 162Bug
 */

/*
#define print(s) outstr(s,s+strlen(s))
*/

#define IVBR 		0x50
#define VBR 		0xFFE00000

#define ABORT_CR 	0xFFF42043
#define MC_CR 		0xFFF42002
#define MC_IVBR		0xFFF42003
#define T1_CR 		0xFFF42017
#define T1_ICR		0xFFF4201B
#define T1_DUREE	0xFFF42004

#define MIEN	2		
#define IEN	0x10
#define LEVEL	3
#define level	4
#define ICLR	8		
#define CEN	1		
#define COC	2	

//int *__udivdi3;  
//int *__umoddi3;
#include "register.h"
#include "vme162.h"


int test;
char *pb;
int *plw;
int numero=0;
void h_abort()
{
pb = (char*)ABORT_CR;
*pb = IEN|ICLR|level;

test=7;

}
void h_timer()
{
pb = (char*)T1_ICR;
*pb = IEN|ICLR|LEVEL;

test=5;

}

		
main()
{
numero=0;
kprint("MVME162 interrupt test V0.0");
//main init
SetHandler(((IVBR+0x09)*4 + VBR), h_timer);// , 0);
SetHandler(((IVBR+0x0E)*4 + VBR), h_abort);// , 16);


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

sti();
test=0;
while(1) {
	kprint(".");
	switch (test) {
		case 7 : kprint("ABORT");
			 test=0; //surtout pas apres le switch !
		         break;
		case 5 : kprint("tick");
			 test=0; //surtout pas apres le switch !
		         break;
		}
		}
}
