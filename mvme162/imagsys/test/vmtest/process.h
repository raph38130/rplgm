/*
 * process.h
 * $Id: process.h,v 1.1 1998/04/08 22:15:22 raph Exp $
 */

#define NBPROC			32 //nbre max de process de E2S
#define taille_PROCESS_NAME	16 //longueur du nom d'un process
#define taille_CONTEXT		3	
#include <stddef.h>


typedef struct process {
   char name[taille_PROCESS_NAME];
   int context[taille_CONTEXT];	//SP USP + Arbre SRP URP
   int pid;
   int prio;
   int status;
   int code;			//debut de la zone code
   int stack;			//bas de la zone pile, qui fait taille_PROCESS_STACK octets
   struct process *suivant;   	// successeur   dans la liste circulaire des procs actifs

   struct process *s_next;	//successeur éventuel dans une des listes :
				//	des WAITING d'un sémaphore
				//	des SLEEPING
				
   void *sem;			//le sémaphore sur lequel il est en attente
   
   int delay;			//pour sleep : nombre de tick avant réveil
   } PROCESS;

//status d'un process
#define ACTIVABLE	1
#define SUSPENDU	2
#define SLEEPING        3
#define INEXISTANT	4
#define WAITING		5

//#define NULL		((PROCESS *) -1)

extern PROCESS PROCS_ARRAY[];

//le process actif (celui qui dispose du temps cpu)
extern PROCESS *ACTIF;

//la liste des process éligibles (activables, de prio égale à cell de ACTIF)
extern PROCESS *READY;

extern int PID;	//le plus grand n° de pid utilisé

//pid 0 = "idle" pas accessible
#define badpid(x) ((x) <= 0 || (x) > PID || PROCS_ARRAY[(x)].status == INEXISTANT)

extern void enleve(PROCESS *);
extern void genere();
