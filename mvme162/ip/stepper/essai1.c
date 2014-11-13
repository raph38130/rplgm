/* RP 25 / 11 / 98
 *
 * mise en oeuvre ip stepper
 *	déplacement avec rampe de démarrage et d'arret
 *	affichage de la position
 * attention :
 *	ne pas compiler avec optimisations!!!
 *	encoder1
 *		A,A* X1
 *		B,B* Y1
 *	stepper1
 *	encodeur HP HEDS9540 (5V)
 *	moteur BERGER LAHR (10V)
 */

#include "register.h"
#include "vme162.h"
#include "stepper.h"

#define IPPIC	0xfffbc000
#define IPAGCR  0x18    /* IP_a general control register */
#define IPAICR0 0x10    /* IP_a interrupt control register 0 */
#define IPAICR1 0x11    /* IP_a interrupt control register 1 */
#define IPAMBAR	0x04	/* IP_a memory base address register */

#define IPABase	0xFFF58000	//IP base Address

#define TICK	500
#define IVBR	80		//comme 162bug

char *pb;
short int *pw;
int *plw;

IP_STEPPER *ip;

void init_IPIC()
{
//init ipic
pw = (short int*)(IPPIC+IPAMBAR);
*pw = IPABase >> 16;	//adresse de base
pb = (char*)IPPIC+IPAGCR;
*pb = 0;		//
pb = (char*)IPPIC+IPAICR0;
*pb = 0;		// ... 
pb = (char*)IPPIC+IPAICR1;
*pb = 0;		// ... 
}

/* génération d'un déplacement avec rampe de démarrage et d'arret
 * (cf doc GreenSpring
 */
void init_stepper()
{
/* les deux bits de poids fort de csr permettent de choisir le registre 
 *	00	START_STOP_CMD
 *	01	OP_MODE_CMD
 *	10	REG_SEL_CMD
 *	11	OUTPUT_MODE_REG
 */
ip->Motor1.csr = START_STOP_CMD | STOP_MOTION;
ip->Motor1.csr = OP_MODE_CMD | EN_CURRENT_POS_CTR | AUTO_RAMP_DOWN | STOP_ON_R0;
ip->Motor1.csr = OUTPUT_MODE_REG |EXTENSION_MODE;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 0; 
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x07;
ip->Motor1.data0 = 0xD0;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 1;
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x00;
ip->Motor1.data0 = 0x14;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 2;
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x01;
ip->Motor1.data0 = 0xF0;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 4;
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x29;
ip->Motor1.data0 = 0x1E;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 5;
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x04;
ip->Motor1.data0 = 0x1D;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 6;
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x00;
ip->Motor1.data0 = 0xDD;
ip->Motor1.csr = REG_SEL_CMD | COLLECTIVE_IO | 7;
ip->Motor1.data2 = 0x00;
ip->Motor1.data1 = 0x01;
ip->Motor1.data0 = 0xE8;

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
static int n=0,m=0;

n=readLS7166(&ip->Encoder1);
printf("position : %05d vitesse : %04d\r",n,n-m);
fflush(0);
m=n;
//acquitement it timer
ack_tick();
}


main()
{
int i,n,m;

init_IPIC();
ip = (IP_STEPPER*)IPABase;
init_stepper();
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
SetHandler(((IVBR+Timer1_IRQ)*4 + VBR), it_timer );

sti();
//start
ip->Motor1.csr = START_STOP_CMD | START_MOTION | FH1_SPEED | RAMP_UP_SPEED;

while(1) stop();
}
