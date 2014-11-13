/* entree16.c
 *
 * Mon Apr 10 12:51:32 CEST 2006 RP
 *	-lecture des entrées de INP16
 *
 * Fri Mar 24 10:22:58 CET 2006 RP
 *	-chenillard 16 leds avec OUT16
export PATH=$PATH:/usr/lib/gcc-lib/m68k-linux/2.95.4/

m68k-linux-gcc -S entree16.c -o entree16.s
m68k-linux-as entree16.s -o entree16.o 
m68k-linux-ld crt0.o entree16.o -o entree16 \
--oformat=srec -Ttext=0x10000 \
-lmvme162 -lc -L ..
 
 *
 */
 
 
#define IP_A	0xFFF58000
#define IP_B	0xFFF58100
#define IP_C	0xFFF58200
#define IP_D	0xFFF58300

#define CR 	(BASE_IP+7)
#define PADR 	(BASE_IP+5)
#define PBDR 	(BASE_IP+3)
#define PCDR 	(BASE_IP+1)

#define BASE_IP	IP_B

#define print(s) outstr(s, s+strlen(s))
unsigned char* pb;
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

int 	i=0;
char 	*pcr, /* cntrl register */
	*pa,  /* port A register */
	*pb;  /* port B register */

DummyGlob = *pb; 		/* put cio in state 0 in case of state 1 */    
WRITE_CIO(MIC_REG,0x00);
READ_CIO(MIC_REG,DummyGlob); 	/* put CIO in state 0*/
WRITE_CIO(MS_REG_A,0x1f);	/* OR priority encoded vecteur mode */

/* init INP16 : port A et B en sortie */
pcr = (char*) CR;
pa  = (char*) PADR;
pb  = (char*) PBDR;

*pcr = 0x23 ; //PADDR
*pcr = 0xFF;
*pcr = 0x2B ; 
*pcr = 0xFF;

*pcr=0x22; //inverting input
*pcr=0xFF;
*pcr=0x2A;
*pcr=0xFF;

*pcr = 0x01; //port A & B enable
*pcr = 0x84;

/* main loop */
print("entree INP16 V1.0\012\015");
while(1) {
	/* afficher */
	print("entree n° ");
	if (*pa & 0x01 == 0x01) print(" 0 ");
	if (*pa & 0x02 == 0x02) print(" 1 ");
	if (*pa & 0x04 == 0x04) print(" 2 ");
	if (*pa & 0x08 == 0x08) print(" 3 ");
	if (*pa & 0x10 == 0x10) print(" 4 ");
	if (*pa & 0x20 == 0x20) print(" 5 ");
	if (*pa & 0x40 == 0x40) print(" 6 ");
	if (*pa & 0x80 == 0x80) print(" 7 ");
	
	
	for(i=0;i<100000;i++);
	print("delai\012\015");
	}


}
