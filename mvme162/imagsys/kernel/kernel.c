/* Projet IMAGSYS 1997-1998
 *
 * kernel.c
 *
 * $Id: kernel.c,v 1.1 1998/04/08 22:15:22 raph Exp $
 *
 * RP
 *  2 / 12 / 97 génération tick scheduler niveau 1
 *              handler it z85230  niveau 2
 *              trap 3 : vmeprint
 *
 * 27 / 12 / 97 gestion des deux ports séries
 *              trap 3 vmeprint1
 *              trap 4 vmeprint0 
 * 29 / 12 / 97 gestion de la lecture sur les ports séries
 *
 * 08 / 01 / 98 réécriture générale respectant mieux l'indépendance vis à vis du processeur et du système
 * 31 / 01 / 98 noyau OK
 * 24 / 02 / 98 primitives + sleep + driver OK
 * 07 / 02 / 98 modification pour permettre la VM ultérieurement
 */

#include "vme162.h"
#include "register.h"
#include "io_call.h"
#include "kernel.h"
#include "schedule.h"

/*********************************************************************
 * INITIALISATION DE E2S 
 *********************************************************************/
#define print(s) outstr(s,s+strlen(s))

main()
{
print("e2s version MMU");
///////////////////////////////////////////
////INITIALISATION DE LA CARTE
//////////////////////////////////////////
/*
 * configurer le MCchip
 */
StoreByte(MCchip_Base + General_Control_R, 0x2);	/* MIEN */
StoreByte(MCchip_Base + SCC_Interrupt_R,   0x12);   	/* IEN it z85230 niveau 2 */

/* 
 * le numero du vecteur d'it du timer 
 * est (InterruptVectorBaseR) + Timer1_IRQ 
 * son adresse est 4 fois plus loin
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
//clock
SetHandler(((IVBR+Timer1_IRQ)*4 + VBR), schedule );
//z85230
SetHandler(0xFF*4 + VBR, handler_Z85230);
//trap fonctions systèmes
SetHandler(  36*4 + VBR, sys_call_select);

//programmer timer MCchip
StoreByte(MCchip_Base+Timer1_Control_R, 3);      	/*CEN COC*/
/* une it toutes les TICK mili secondes */
StoreLong(MCchip_Base+Timer1_Compare_R, TICK*1000);	
StoreByte(MCchip_Base+Timer1_Interrupt_R, 0x11);   	/*IEN  IL=1*/

////////////////////////////////////////////
//// INITIALISATION POUR LE NOYAU
///////////////////////////////////////////
init_mode_S(); 		//init en mode S
//démasquer les it
sti();
// GO !
while (1) {
   stop();  //pour libérer le bus 
   }
}
/*
 * end 
 */
