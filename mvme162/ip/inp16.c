/* 
 * Mise en oeuvre INP16 en scrutation sans OS
 *
 *	RP Mon Nov 20 22:47:12 CET 2000
 *	version verifiée correcte
 */
 
//Z8536
#define	MIC_REG		0x0	/* Master Interrupt Control register */
#define	MCC_REG		0x1	/* Master Configuration Control register */
#define	IV_REG_A	0x2	/* PortA Interrupt Vector register */
#define	CS_REG_A	0x8	/* PortA Command and Status register */
#define	DATA_REG_A	0xd	/* PortA data register */
#define	MS_REG_A	0x20	/* Port A Mode Specification register */
#define	HS_REG_A	0x21	/* Port A Handshacke Specification register */
#define	DPP_REG_A	0x22	/* Port A Data Path Polarity register */
#define	DD_REG_A	0x23	/* Port A Data Direction register */
#define	SC_REG_A	0x24	/* Port A Special I/O Control register */
#define	PP_REG_A	0x25	/* Port A Pattern Polarity register */
#define	PT_REG_A	0x26	/* Port A Pattern Transition register */
#define	PM_REG_A	0x27	/* Port A Pattern Mask register */

//INP16
#define IP_A	0xFFF58000
#define IP_B	0xFFF58100
#define BASE_IP	IP_B

#define CR 	(BASE_IP+7)
#define PADR 	(BASE_IP+5)


unsigned char* pb;
int N=0;

/* macro definition for cio register access */

#define WRITE_CIO(reg,val)\
{\
      pb = (unsigned char*) CR;\
      *pb = reg;\
      *pb = val;\
}

#define READ_CIO(reg,val)\
{\
      pb = (unsigned char*) CR;\
      *pb = reg;\
      val = *pb;\
}


main()
{
int DummyGlob;

kprintf("INP16 v1.0.0\n");
     
//init cio
asm("move.w #0x2700,%sr");
pb = (unsigned char*) CR;
DummyGlob = *pb; 		/* put cio in state 0 in case of state 1 */    
WRITE_CIO(MIC_REG,0x00);
READ_CIO(MIC_REG,DummyGlob); 	/* put CIO in state 0*/

WRITE_CIO(MS_REG_A,0x1f);	/* OR priority encoded vecteur mode */
WRITE_CIO(DPP_REG_A,0x0); 	/* non inverting inputs */

WRITE_CIO(PP_REG_A,0x00);
WRITE_CIO(PT_REG_A,0x00);
WRITE_CIO(PM_REG_A,0x00);

WRITE_CIO(DD_REG_A,0xFF); 	//port A Data Direction Register
WRITE_CIO(MCC_REG,0x4); 	//port A Enable
	
asm("move.w #0x2000,%sr");

kprintf("select an input\n\r");
pb = (unsigned char*) PADR;
while(1) 
  switch(*pb) {
	case (0xFE): kprintf("	0\012\015");N=0;break;
	case (0xFd): kprintf("	1\012\015");N=0;break;
	case (0xFb): kprintf("	2\012\015");N=0;break;
	case (0xF7): kprintf("	3\012\015");N=0;break;
	case (0xEF): kprintf("	4\012\015");N=0;break;
 	case (0xdF): kprintf("	5\012\015");N=0;break;
	case (0xbf): kprintf("	6\012\015");N=0;break;
	case (0x7f): kprintf("	7\012\015");N=0;break;
   }
}



