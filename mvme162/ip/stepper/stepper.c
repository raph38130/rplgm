/*
 * 
 * RP sam avr  9 10:11:09 UTC 2005
 *	- 
 *
 * 
 *	encoder1		HEDS	IP-STEPPER
 *		A,A* X1 	5	3
 *		B,B* Y1		6	2
 *				7	7
 *				8	8
 *	stepper1
 *	encodeur HP HEDS9540 (5V)

 *
 */



#include "register.h"
#include "vme162.h"
#include "stepper.h"

#define IVBR 		0x50
#define VBR 		0xFFE00000
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

#define TICK	1000


int n=0,val;
char *pb;
short int *pw;
int *plw;
int numero=0;

IP_STEPPER *ip;

/*
 * sauvegarde par tftp (primitive 16xBug)
 */

#include "net.h"
#include "types.h"

#define N	1000
int T[N];

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

void h_timer()
{
//kprint("top");
//T[n++]=readLS7166(&ip->Encoder1);
val=readLS7166(&ip->Encoder1);
//n++;

ack_tick();
}

		
main()
{
char buffer[30];
n=0;
numero=0;

kprint("MVME162 IP-STEPPER V2.0");

ip = (IP_STEPPER*)IPCBase;
ip->Encoder1.control = MASTER_CTL_REG | MASTER_RESET;
ip->Encoder1.control = INPUT_CTL_REG | ENABLE_AB ;
ip->Encoder1.control = OUTPUT_CTL_REG | BINARY_COUNT | WRAP_COUNT | COMP_MATCH;
ip->Encoder1.control = QUAD_REG  | QUAD_X4;
ip->Encoder1.control = MASTER_CTL_REG | COUNTER_RESET;

ip->Motor1.csr = START_STOP_CMD | STOP_MOTION;
ip->Motor1.csr = OP_MODE_CMD | EN_CURRENT_POS_CTR | AUTO_RAMP_DOWN | STOP_ON_R0;
ip->Motor1.csr = OUTPUT_MODE_REG |EXTENSION_MODE;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 0; // steps number
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x40;
ip->Motor1.data0 = 0xD0;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 1; // initial speed
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x00;
ip->Motor1.data0 = 0xF0;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 2; // high speed
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x05; 
ip->Motor1.data0 = 0x80;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 4; // clk / accel
ip->Motor1.data2 = 0x01;
ip->Motor1.data1 = 0x00;
ip->Motor1.data0 = 0x00;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 5; // clk / decel
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x00;
ip->Motor1.data0 = 0x0F;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 6; // ramp down steps
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x02;
ip->Motor1.data0 = 0x00;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 7; // clck
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x01;
ip->Motor1.data0 = 0xE8;

//programmer timer MCchip : une it toutes les TICK mili secondes
StoreByte(MCchip_Base + General_Control_R, 0x2);	/* MIEN 	*/
StoreByte(MCchip_Base+Timer1_Control_R, 3);      	/* CEN COC 	*/
StoreLong(MCchip_Base+Timer1_Compare_R, TICK*50);	
StoreByte(MCchip_Base+Timer1_Interrupt_R, 0x11);   	/* IEN IL=1 	*/
SetHandler(((IVBR+Timer1_IRQ)*4 + VBR), h_timer );


kprint("init done\015\012");


sti();
//while(1) {
	ip->Motor1.csr = START_STOP_CMD | START_MOTION | FH1_SPEED | RAMP_UP_SPEED;
/*	kprint("          test ");
	itoa(val,buffer);
	kprint(buffer);
	kprint("\015\012");
*/
/*	if (n>N) break;
	//if (T[n-1]==T[n-2] && T[n-1]==T[n-3] && T[n-1]==T[n-4] && T[n-1]==T[n-5]) break;
*/
//	stop();
//	}
/*
kprint("saving\015\012");
save((char*)T,n*4,"data");
*/
kprint("bye");
}
