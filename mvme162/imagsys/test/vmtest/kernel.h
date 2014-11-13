/*
 * $Id*
 */

#define TICK		33		//en milisecondes

//emplacement des variables systemes (pour shell)
#define ENVIRON		(int*) 0x9000	
#define indice_PROCS	0
#define indice_SEMA	1
#define	indice_SLEEP	2
#define	indice_FD       3
#define	indice_DEV      4
#define	indice_TTY      5
