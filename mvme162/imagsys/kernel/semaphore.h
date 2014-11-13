/*
 * $Id: semaphore.h,v 1.1 1998/04/08 22:15:22 raph Exp $
 * sémaphores  inspiré des "évènements" OS9
 */



#define NSEMA 	30
#define VALID 	111
#define FREE	222
#define Taille_Semaphore_Name	20

#define isbadsema(s) 	(  ((s) < &(SEMA_ARRAY[0]) ) 	\
			|| ((s) > &(SEMA_ARRAY[NSEMA]) )\
			|| ((s)->s_status != VALID) )

typedef
struct semaphore {
        char s_name[Taille_Semaphore_Name];
	int s_count;
	PROCESS *s_list;
	int s_status;	//VALID FREE
        } SEMAPHORE;

extern SEMAPHORE SEMA_ARRAY[];
