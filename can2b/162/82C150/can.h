/* RP 11 / 98
 * 
 * 
 */
 
//IPIC
#define IPPIC	0xfffbc000
#define IPAGCR  0x18    /* IP_a general control register */
#define IPAICR0 0x10    /* IP_a interrupt control register 0 */
#define IPAICR1 0x11    /* IP_a interrupt control register 1 */
#define IPAMBAR	0x04	/* IP_a memory base address register */
//IP base Adress
#define IPABase	0xFFF58000
//i82527

#define VectorReg	0x101
#define IRQ_RX		0xF0

extern char *pb;
extern short int *pw;
extern int *plw;

extern i82527_REGS *can;
extern void init_IPIC();
extern void init_tip816();
extern void init_MsgObjects();
extern void end_init();
