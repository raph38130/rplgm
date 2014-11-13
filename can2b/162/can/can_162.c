/*
 * RP 10 / 11 / 98
 *
 * mise en oeuvre ip can tip816/10
 *
 */

//s buffer de N octets
//renvoie le nombre de car lus
int instr(char N,char *s)
{
asm volatile("move.b %0,(%1); move.l %1,-(%%a7); trap #15; .word 3; move.b (%1),%0" :
    "=g" (N) :
    "a" (s), "d" (N) :
    "memory" );
return (N & 0xFF);
}


//IPIC
#define IPPIC	0xfffbc000
#define IPAGCR  0x18    /* IP_a general control register */
#define IPAICR0 0x10    /* IP_a interrupt control register 0 */
#define IPAICR1 0x11    /* IP_a interrupt control register 1 */
//IP base Adress
#define IPABase	0xFFF58000
//i82527
#include "i82527.h"

#define VectorReg		0x101
#define ControlReg		0x00
#define StatusReg		0x01
#define CPUInterfaceReg		0x02
#define GlobalMaskStandard	0x06
#define GlobalMaskExtended	0x08
#define Message15Mask		0x0C
#define BusConfigReg		0x2F
#define BitTimingReg0		0x3F
#define BitTimingReg1		0x4F

#define NREGS	14
char *regs[]={"cr","sr","CPUr","GMS","GMS","GME","GME","GME","GME","Clk","Bus","btr0","btr1","ir"};
char regnum[]={0  , 1  ,  2    ,  6,    7  ,  8,    9,   10,   11  , 0x1F, 0x2F,0x3F, 0x4F,0x5F};
char *msgregs[]={"cr0","cr1","a0","a1","a2","a3","conf","b0","b1","b2","b3","b4","b5","b6","b7"};

char buffer[80];
char *pb;
short int *pw;
int *plw;

MSGOBJ msgobject[MAX_MSGOBJS];


init()
{
int i,j;
//init ipic pour tip816
//les autres valeurs ont été initialisées par ENV de 162Bug
pb = (char*)IPPIC+IPAGCR;
*pb = 9;	//IPMEN IPWIDTH_16
pb = (char*)IPPIC+IPAICR0;
*pb = 0;//IEN level ... 
pb = (char*)IPPIC+IPAICR1;
*pb = 0;//IEN level ... 

//init pour i82527
pb = (char*)IPABase+VectorReg;
*pb = 0x60;
pb = (char*) IPABase+ControlReg;
*pb = icCCE | icInit;	//icEIE | icIE | 
pb = (char*) IPABase+StatusReg;
*pb = 0x07;
pb = (char*) IPABase+CPUInterfaceReg;
*pb = 0;
pw = (short int*) (IPABase+GlobalMaskStandard);
*pw = 0xFFFF;
plw = (int*) (IPABase+GlobalMaskExtended);
*plw = 0xFFFFFFFF;
plw = (int*) (IPABase+Message15Mask);
*plw = 0xFFFFFFFF;
pb = (char*) IPABase+BusConfigReg;
*pb = icDcT1;
pb = (char*) IPABase+BitTimingReg0;
*pb =  0x47;
pb = (char*) IPABase+BitTimingReg1;
*pb =  0x2F;

  /* ------------------------------ */
  /* initialize all message objects */
  /* ------------------------------ */
  
  for (i=0; i<MAX_MSGOBJS; i++)
  {
	msgobject[i].Control0 =	RESET_MsgVal &
				RESET_TXIE   &
				RESET_RXIE   &
				RESET_IntPnd;

	msgobject[i].Control1 = RESET_RmtPnd &
				RESET_TxRqst &
				RESET_MsgLst &
				RESET_NewDat;

	msgobject[i].Arbitration   = 0;
	msgobject[i].MessageConfig = 0;
	for (j = 0; j < 8; j++)
		msgobject[i].data[j] = 0;
  }
  /*
   * disable write access to configuration register and disable software
   * initialization.
   */
pb = (char*) IPABase+ControlReg;
*pb &= (~icCCE & ~icInit);

}

ReadMsgObjetct(int n)
{
int i;
if(n<1 || n>15) return;
printf("\n");
pb = (char*) IPABase;
for(i=0;i<15;i++) 
	printf("%2d %s\t\t : %x\n",n*16+i,msgregs[i],*(pb+n*16+i) & 0xFF);


}

main() 
{
int i,j,
	r=0,d,n;

init();

while(1)
{
pb = (char*) IPABase;
for(i=0;i<NREGS;i++) 
	printf("%2d %s\t\t : %x\n",regnum[i],regs[i],*(pb+regnum[i]) & 0xFF);

printf("N°reg. :\n");
if(instr(80,buffer)>0) {
	if(buffer[1]=='m') {
		sscanf(buffer+2,"%d",&r);
		ReadMsgObjetct(r);
		}
	else sscanf(buffer+1,"%d",&r);
	}
pb = (char*) (IPABase+r);
printf("data :\n");
if(instr(80,buffer)>0) {
	sscanf(buffer+1,"%x",&d);
	printf("adresse : %x data %x\n",pb,d & 0xFF);
	*pb = d;
	}
}


}
