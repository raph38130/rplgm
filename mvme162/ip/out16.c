/*
 * Actis IP Modules OUT16 (z8536)
 *
 * RP Sat Nov 18 20:50:50 CET 2000
 * 	version correcte et verifiée
 *	OUT16 sur port A
 *	nappe sur le connecteur le plus éloigné de la face avant
 * 	switch côté connecteur
 * RP Wed Mar 24 20:19:04 CET 1999
 *
 *
 * l'accès aux registres du CIO se fait en deux étapes (macro WRITE/READ_CIO)
 * procédure particulière d'initialisation
 * le module ip mappe directement PADR PBDR PCDR
 * 
 * info sur z8536 dans icv196.c
 */
 
#define IP_A	0xFFF58000
#define IP_B	0xFFF58100
#define BASE_IP	IP_B

#define CR 	(BASE_IP+7)
#define PADR 	(BASE_IP+5)

/* macro definition for cio register access */
#define WRITE_CIO(reg,val)\
{\
      pb = (char*)CR;\
      *pb = reg;\
      *pb = val;\
}

#define READ_CIO(reg,val)\
{\
      pb = (char*)CR;\
      *pb = reg;\
      val = *pb;\
}


void delai()
{
long int i;
for(i=0;i<100000;i++);
}

int DummyGlob;
unsigned char *pb;
unsigned short int *pw;

main() 
{
int i;

//kprintf("OUT16 v0.0.2\n");
//detect IP
//printf("IP base = 0x%x\n",BASE_IP);
//pw = (unsigned short int*) (BASE_IP+0x80);
//for(i=0;i<4;i++) printf("%c",*(pw+i));             

             
//init
pb = (char*)CR;
DummyGlob = *pb; 		/* put cio in state 0 in case of state 1 */   
WRITE_CIO(0x0,0x00);
READ_CIO(0x0,DummyGlob); 	/* put CIO in state 0*/

WRITE_CIO(0xd,0xF); 		//port A Data Register
WRITE_CIO(0x1,0xF4); 		//port Enable

//main loop : clignotant
pb = (char*)PADR;
while(1) {
	//kprintf("tick\012\015");
	*pb = 0x0F;		//accès direct au registre
	delai();
	*pb = 0xF0;
	delai();
	}
}
