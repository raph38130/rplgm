/* RP 25 / 11 / 98
 *
 * mise en oeuvre ip stepper
 *	étude de l'oscillation d'un pendule
 * attention :
 *	ne pas compiler avec optimisations!!!
 *	encoder1
 *		A,A* X1
 *		B,B* Y1
 *	stepper1
 *	encodeur HP HEDS9540 (5V)
 
 
/usr/bin/m68k-linux-ld  -Ttext=0x10000  \
--oformat binary \
-Map map        \
crt0.o essai2.o ls7166.o trap_handler.o  \
 -o /tftpboot/vmeboot
 
 
 
 
 
 -l68k           \
-L /disk/c68k/lib/ -lnewlibc -lm\
-L /disk/c68k/lib/gcc-lib/m68k-coff/2.7.2.l.3/msoft-float -lgcc\

 /disk/c68k/lib/ensimag/mvme162lx.o
 
 
 
 */
int *__udivdi3; 
int *__umoddi3;
#include "register.h"
#include "vme162.h"
#include "stepper.h"

#define IPPIC	0xfffbc000
#define IPAGCR  0x18    /* IP_a general control register */
#define IPAICR0 0x10    /* IP_a interrupt control register 0 */
#define IPAICR1 0x11    /* IP_a interrupt control register 1 */
#define IPAMBAR	0x04	/* IP_a memory base address register */

#define IPCGCR  0x1A    /* IP_a general control register */
#define IPCICR0 0x14    /* IP_a interrupt control register 0 */
#define IPCICR1 0x15    /* IP_a interrupt control register 1 */
#define IPCMBAR	0x08	/* IP_a memory base address register */

#define IPABase	0xFFF58000	//IP A base Address
#define IPCBase	0xFFF58200	//IP C base Address

#define TICK	10
#define IVBR	80		//comme 162bug

char *pb;
short int *pw;
int *plw;

IP_STEPPER *ip;

#define N	1000
int T[N],n=0;

char buf[80];


/*
 * sauvegarde par tftp (primitive 16xBug)
 */

#include "net.h"
#include "types.h"

//renvoie ... en cas d'erreur
int save(char*adr, int length, char*file)
{
netrw_blk_16xbug b;

b.ctrl_lun=b.dev_lun=0;
b.max_length=length;b.offset=0;//tout le fichier
b.addr=(int)adr;
strcpy(b.filename,file);
return netwr_16xbug(&b);
}






void init_IPIC()
{
//init ipic
pw = (short int*)(IPPIC+IPCMBAR);
*pw = IPABase >> 16;	//adresse de base
pb = (char*)IPPIC+IPCGCR;
*pb = 0;		//
pb = (char*)IPPIC+IPCICR0;
*pb = 0;		// ... 
pb = (char*)IPPIC+IPCICR1;
*pb = 0;		// ... 
}

/*
 * mode Quadrature *4
 */
void init_encoder()
{
ip->Encoder1.control = MASTER_CTL_REG | MASTER_RESET;
ip->Encoder1.control = INPUT_CTL_REG | ENABLE_AB ;
ip->Encoder1.control = OUTPUT_CTL_REG | BINARY_COUNT | WRAP_COUNT | COMP_MATCH;
ip->Encoder1.control = QUAD_REG | QUAD_X4;
ip->Encoder1.control = MASTER_CTL_REG | COUNTER_RESET;
}

void it_timer()
{
//T[n++]=readLS7166(&ip->Encoder1);
kprint("top");
n++;
//acquitement it timer
ack_tick();
}


main()
{
int i;

kprint("IP-stepper V1.1");
init_IPIC();
ip = (IP_STEPPER*)IPCBase;
init_encoder();
//
ip->AxisCtrl1.Polarity = 0; 
ip->AxisCtrl1.Source = 0;
ip->SyncCtrl = 0;
ip->intVector = 0xF0;

//programmer timer MCchip : une it toutes les TICK mili secondes
StoreByte(MCchip_Base + General_Control_R, 0x2);	/* MIEN 	*/
StoreByte(MCchip_Base+Timer1_Control_R, 3);      	/* CEN COC 	*/
StoreLong(MCchip_Base+Timer1_Compare_R, TICK*1000);	
StoreByte(MCchip_Base+Timer1_Interrupt_R, 0x11);   	/* IEN IL=1 	*/
kprint("before ?\n");
SetHandler(((IVBR+Timer1_IRQ)*4 + VBR), it_timer );
kprint("after ?\n");

n=0;
kprint("ready ?\n");
//instr(80,buf); 
sti();
//on attend la stabilisation
//while(n<5);
while(1) {
	if (n>N) break;
	if (T[n-1]==T[n-2] && T[n-1]==T[n-3] && T[n-1]==T[n-4] && T[n-1]==T[n-5]) break;
	//stop(); 
	}
cli();	
//kprint("%d échantillons\n",n);
kprint("sauvegarde sous /tftpdir/data\n");
save((char*)T,n*4,"data");
}
