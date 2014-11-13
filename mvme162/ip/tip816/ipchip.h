/*****************************************************************************/
/******************************************************************************

	ipchip.h

	IPchip Programming Model Definitions.


******************************************************************************/
/*****************************************************************************/

/*
 * IPchip Control/Status Registers
 *
 * Offsets from base register
 */
#define IPCIDR	0x00	/* chip identifier register */
#define IPCRR	0x01	/* chip revision register */
#define IPAMBAR	0x04	/* IP_a memory base address register */
#define IPBMBAR	0x06	/* IP_b memory base address register */
#define IPCMBAR	0x08	/* IP_c memory base address register */
#define IPDMBAR	0x0A	/* IP_d memory base address register */
#define IPAMSR	0x0C	/* IP_a memory size register */
#define IPBMSR	0x0D	/* IP_b memory size register */
#define IPCMSR	0x0E	/* IP_c memory size register */
#define IPDMSR	0x0F	/* IP_d memory size register */
#define IPAICR0	0x10	/* IP_a interrupt control register 0 */
#define IPAICR1	0x11	/* IP_a interrupt control register 1 */
#define IPBICR0	0x12	/* IP_b interrupt control register 0 */
#define IPBICR1	0x13	/* IP_b interrupt control register 1 */
#define IPCICR0	0x14	/* IP_c interrupt control register 0 */
#define IPCICR1	0x15	/* IP_c interrupt control register 1 */
#define IPDICR0	0x16	/* IP_d interrupt control register 0 */
#define IPDICR1	0x17	/* IP_d interrupt control register 1 */
#define IPAGCR	0x18	/* IP_a general control register */
#define IPBGCR	0x19	/* IP_b general control register */
#define IPCGCR	0x1A	/* IP_c general control register */
#define IPDGCR	0x1B	/* IP_d general control register */
#define IPRSTR	0x1F	/* IP reset register */


/*
 * IPchip Control/Status Registers
 *
 * Structure definitions
 */
typedef struct ipic {
	unsigned char	ipcidr;     /* chip identifier register */
	unsigned char	ipcrr;      /* chip revision register */
	unsigned char	void00[2];  /* void */
	unsigned short	ipambar;    /* IP_a memory base address register */
	unsigned short	ipbmbar;    /* IP_b memory base address register */
	unsigned short	ipcmbar;    /* IP_c memory base address register */
	unsigned short	ipdmbar;    /* IP_d memory base address register */
	unsigned char	ipamsr;     /* IP_a memory size register */
	unsigned char	ipbmsr;     /* IP_b memory size register */
	unsigned char	ipcmsr;     /* IP_c memory size register */
	unsigned char	ipdmsr;     /* IP_d memory size register */
	unsigned char	ipaicr0;    /* IP_a interrupt control register 0 */
	unsigned char	ipaicr1;    /* IP_a interrupt control register 1 */
	unsigned char	ipbicr0;    /* IP_b interrupt control register 0 */
	unsigned char	ipbicr1;    /* IP_b interrupt control register 1 */
	unsigned char	ipcicr0;    /* IP_c interrupt control register 0 */
	unsigned char	ipcicr1;    /* IP_c interrupt control register 1 */
	unsigned char	ipdicr0;    /* IP_d interrupt control register 0 */
	unsigned char	ipdicr1;    /* IP_d interrupt control register 1 */
	unsigned char	ipagcr;     /* IP_a general control register */
	unsigned char	ipbgcr;     /* IP_b general control register */
	unsigned char	ipcgcr;     /* IP_c general control register */
	unsigned char	ipdgcr;     /* IP_d general control register */
	unsigned char	void01[3];  /* void */
	unsigned char	iprstr;     /* IP reset register */
} IPIC;


/*
 * IPchip Control/Status Registers
 *
 * Bit definitions
 */

/*
 * the following registers use one or more of the following register
 * bit definitions
 */
/* IPAICR0/IPAICR1 (ipaicr0/ipaicr1): IP_a interrupt control registers */
/* IPBICR0/IPBICR1 (ipbicr0/ipbicr1): IP_b interrupt control registers */
/* IPCICR0/IPCICR1 (ipcicr0/ipcicr1): IP_c interrupt control registers */
/* IPDICR0/IPDICR1 (ipdicr0/ipdicr1): IP_d interrupt control registers */
#define IP_IL0	(1<<0)	/* interrupt level bit #0 */
#define IP_IL1	(1<<1)	/* interrupt level bit #1 */
#define IP_IL2	(1<<2)	/* interrupt level bit #2 */
#define IP_ICLR	(1<<3)	/* interrupt clear */
#define IP_IEN	(1<<4)	/* interrupt enable */
#define IP_INT	(1<<5)	/* interrupt */
#define IP_EL	(1<<6)	/* edge/level sensitive */
#define IP_PLTY	(1<<7)	/* polarity */

#define IP_IRQ0	0			/* polled interrupt level */
#define IP_IRQ1	(IP_IL0)		/* interrupt level 1 */
#define IP_IRQ2	(IP_IL1)		/* interrupt level 2 */
#define IP_IRQ3	(IP_IL1|IP_IL0)		/* interrupt level 3 */
#define IP_IRQ4	(IP_IL2)		/* interrupt level 4 */
#define IP_IRQ5	(IP_IL2|IP_IL0)		/* interrupt level 5 */
#define IP_IRQ6	(IP_IL2|IP_IL1)		/* interrupt level 6 */
#define IP_IRQ7	(IP_IL2|IP_IL1|IP_IL0)	/* interrupt level 7 */

/*
 * the following registers use one or more of the following register
 * bit definitions
 */
/* IPAGCR (ipagcr): IP_a general control register */
/* IPBGCR (ipbgcr): IP_b general control register */
/* IPCGCR (ipcgcr): IP_c general control register */
/* IPDGCR (ipdgcr): IP_d general control register */
#define IPMEN	(1<<0)	/* memory space access enable */
#define IPWIDTH_32	(0<<2)	/* memory space data width = 32 bits */
#define IPWIDTH_8	(1<<2)	/* memory space data width =  8 bits */
#define IPWIDTH_16	(2<<2)	/* memory space data width = 16 bits */
#define IPWIDTH_R	(3<<2)	/* reserved */
#define IPRT_0	(0<<4)	/* recovery timer = 0 uSec */
#define IPRT_2	(1<<4)	/* recovery timer = 2 uSec */
#define IPRT_4	(2<<4)	/* recovery timer = 4 uSec */
#define IPRT_8	(3<<4)	/* recovery timer = 8 uSec */
#define IPERR	(1<<7)	/* Industry Pack Error */

/* IPRSTR (iprstr): IP reset register */
#define IP_RES	(1<<0)	/* IP reset */
