/* k2000.c
 *
 * Fri Mar 24 10:22:58 CET 2006 RP
 *	-chenillard 16 leds avec OUT16
 
m68k-linux-gcc -S k2000.c -o k2000.s
m68k-linux-as k2000.s -o k2000.o 
m68k-linux-ld crt0.o k2000.o -o k2000 \
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

main()
{
int 	i=0;
char 	*pcr, /* cntrl register */
	*pa,  /* port A register */
	*pb;  /* port B register */

/* init OUT16 : port A et B en sortie */
pcr = (char*) CR;
pa  = (char*) PADR;
pb  = (char*) PBDR;

*pcr = 0x23 ; //PADDR=0 (sens=sortie)
*pcr = 0x00;
*pcr = 0x2B ; //PBDDR=0 (sens=sortie)
*pcr = 0x00;

*pcr = 0x22; //
*pcr = 0x00;
*pcr = 0x2A;
*pcr = 0x00;



/* main loop */
print("chenillard V1.0\012\015");
while(1) {

	/* allumer led n°i */
	*pa = ...;
	*pb = ...;
	
	/* attendre un peu */
	for(j=0 ; j<1000000 ; j++) ;
	
	i=(i+1) % 16;
	}


}
