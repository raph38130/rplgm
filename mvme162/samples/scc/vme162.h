/*
 * MCchip
 */
#define MCchip_Base		0xFFF42000
#define General_Control_R 	0x2
#define Interrupt_Vector_Base_R 0x3
#define Timer1_Compare_R	0x4
#define Timer1_Counter_R	0x8
#define Timer1_Control_R	0x17
#define Timer1_Interrupt_R	0x1B
#define Timer1_IRQ		9
#define SCC_Interrupt_R		0x1D

/*
 * Z85230 : 4 ports /term /t1 (/t2 /t3)
 */
#define Z85230_0		0xFFF45005
#define Z85230_1		0xFFF45001
#define Z85230_2		0xFFF45805
#define Z85230_3		0xFFF45801

/*
 * Intel TOD
 */
#define SECONDS			0xFFFC1FF9
#define MINUTES			0xFFFC1FFA
#define HOUR			0xFFFC1FFB
#define DAY			0xFFFC1FFC
#define DATE			0xFFFC1FFD
#define MONTH			0xFFFC1FFE
#define YEAR			0xFFFC1FFF


#define StoreByte(adresse, n)		\
	asm volatile("move.b %0,(%1)" : : "g" (n) , "a" (adresse) : "memory" )
#define ReadByte(adresse, n)		\
	asm volatile("move.b (%1),%0" : "=g" (n) : "a" (adresse) : "memory" )
