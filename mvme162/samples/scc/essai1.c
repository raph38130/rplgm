/*
 * gestion z85230 par it
 * 
 * RP 06 / 98
 */


#include "vme162.h"

#define Taille_Buffer 256

#define CR	0xD
#define LF	0xA

//SCC WR0
#define ResetHighestIUS		0x38
#define ResetTxItPending	0x28
#define EnableItOnNextRx	0x20
//SCC RR3
#define tbe_1	2
#define rbf_1	4
#define tbe_0	16
#define rbf_0	32
#define err_0	8
#define err_1	1

char I_BUFFER[Taille_Buffer];
char O_BUFFER[Taille_Buffer];
int i=0,o=0;

int SCC[2]={Z85230_0, Z85230_1};


void lire(int port)
{
char c;
ReadByte(SCC[port]+2, c);
StoreByte(SCC[port], ResetHighestIUS);
//echo, CR-CR+LF
if (c==CR) StoreByte(SCC[port]+2,LF);
StoreByte(SCC[port]+2,c);
//StoreByte(SCC[port], ResetTxItPending);
}

void ecrire(int port)
{
StoreByte(SCC[port], ResetHighestIUS);
StoreByte(SCC[port]+2,O_BUFFER[o]);
}



//version avec handler commun à toutes les it
void handler_Z85230()
{
char c;

asm("movem.l %d0-%d1/%a0-%a1,-(%sp)");
//déterminer la ou les causes de l'it par la lecture de RR3
l1 :
StoreByte(Z85230_0,3);
ReadByte(Z85230_0,c);
if (c & tbe_0) {ecrire(0); goto l1;}
if (c & rbf_0) {lire(0); goto l1;}
if (c & tbe_1) {ecrire(1); goto l1;}
if (c & rbf_1) {lire(1); goto l1;}
if (c & err_0) {StoreByte(SCC[0], ResetHighestIUS);
		//print("erreur reception tty0");
		}
if (c & err_1) {StoreByte(SCC[1], ResetHighestIUS);
		//print("erreur reception tty1");
		}
asm("movem.l (%sp)+,%d0-%d1/%a0-%a1
     rte
     ");
}

main()
{
int* plw;
/*
 * configurer le MCchip
 */
StoreByte(MCchip_Base + General_Control_R, 0x2);	/* MIEN */
StoreByte(MCchip_Base + SCC_Interrupt_R,   0x12);   	/* IEN it z85230 niveau 2 */

/* 
 * on garde IVBR (MCchip) de 162Bug
 */
#define IVBR	80
StoreByte(MCchip_Base+Interrupt_Vector_Base_R, IVBR);

/* 
 * configurer le Z85230_1 Write + Read
 *               Z85230_0 Write + Read
 */
//on pourrait rajouter le paramétrage de la ls 9600 N 8 1
StoreByte(Z85230_1, 0x01);	/* pour acceder à WR1      				*/
StoreByte(Z85230_1, 0x12); 	/* Int on All Rx + Tx Int Enable 			*/
StoreByte(Z85230_1, 0x09);  	/* pour acceder à WR9      				*/
StoreByte(Z85230_1, 0x08);  	/* MIE Status Low + mais pas VIS 				*/
StoreByte(Z85230_1, 0x02);  	/* pour acceder à WR2      				*/
StoreByte(Z85230_1, 0xFF); 	/* vecteur d'it n° 0xFF  niveau de priorité : 2 	*/

StoreByte(Z85230_0, 0x01);  	/* pour acceder à WR1      				*/
StoreByte(Z85230_0, 0x12); 	/* Int on All RX + Tx Int Enable 			*/
/* les autres registres sont communs				*/

/* 
 * installer les vecteurs d'it
 */
plw = (int*) (0xFF*4+VBR);
*plw = (int)handler_Z85230+4;

//demasquer les IT
asm("move.w #0x2000,%sr");

i=o=0;
while (1) ;
}
