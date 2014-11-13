/*
 * $Id$
 *
 *
 */
#include "vme162.h"

#define nIT_Vector		10 //nombre maximum de handler 



extern int numero;
char  VECTOR[0x50]; 


void SetHandler(int adresse, void f())  //,int numero)
{
int 		*plw;
short int 	*pw;

if (numero > 24*nIT_Vector) 
   //panic !
   while(1) ;
//installer le handler
plw = (int*) &(VECTOR[numero]);
*plw++ = 0x48E7C0C0; 		//movem.l d0-d1/a0-a1,-(sp)
pw = (short int *) plw;
*pw++ = 0x4EB9; 		//jsr
plw = (int *) pw;
*plw++ = (int)  f;		//    addresse_handler 
*plw++ = 0x4CDF0303; 		//movem.l (sp)+,d0-d1/a0-a1
pw = (short int *) plw;
*pw = 0x4E73; 			//rte

//mettre a jour le vecteur
plw = (int *) adresse;
*plw = (int) &(VECTOR[numero]);

numero += 16;
}
