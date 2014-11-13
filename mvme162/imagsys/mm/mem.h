/*
 * $Id$
 */
#define print(s) outstr(s,s+strlen(s))

//caractéristiques d'une zone process
#define TAILLE  	4096	//1 page par process pour les piles
#define PROCESS_STACK	2000	//offset pile utilisateur	
#define K_STACK		4000	//offset pile appels systèmes

//allocation mémoire par page repérées par tableau de bits
#define DRAM	10
#define SRAM	20
#define TAILLE_DRAM	0x400000	//4Mo
#define TAILLE_SRAM	 0x20000	//128ko
#define SRAM_BASE	0xFFE00000

#define TAILLE_PAGE     4096
#define NB_BITS_PAR_MOTS 32
#define TAILLE_BITMAP_PAGES_ALLOUEES_DRAM	(TAILLE_DRAM / (TAILLE_PAGE * NB_BITS_PAR_MOTS))
#define TAILLE_BITMAP_PAGES_ALLOUEES_SRAM	(TAILLE_SRAM / (TAILLE_PAGE * NB_BITS_PAR_MOTS))

extern char *PageAlloc(int);
extern void PageFree(char *, int);
extern void init_mm();

//MMU VM
typedef unsigned long int  paddr_t;
#define NIV1_2 0
#define NIV3   1
#define TRANSPARANT	(1<<3)
#define MODE_S		(1<<4)

struct desc_ptp
   {
   unsigned int adr;         /*   adresse physique de la page                 */
   unsigned int valid : 1;   /*   descripteur valide ou non                   */
   unsigned int type : 1;    /*   page pour tables de niveaux (1 et 2) ou 3   */
   unsigned int : 14;        /*   inutilise                                   */
   unsigned int alloue : 16; /*   indicateur de table allouee                 */
   };
/*   tableau des descripteurs de page de tables de pages   */
#define NB_DESC_PTP 100
struct desc_ptp tdesc_ptp[NB_DESC_PTP];

/*   descripteur dans les tables de pages   */
typedef int DESCR;


//allocation par le "buddy system"
#define BASE 		0x200000	//début du tas
#define log2heapsize	20		//sa taille : 1 M. Octets



/* $Id$ */
/*
h	buddy.h:
	dynamic mem management with the buddy system
impl	ADI C
log	14.03.95/Rolf: creation
$Log$
*/

#include <stddef.h>

#define FALSE	0
#define TRUE	1

typedef struct freeListNode {
 int size;					/* if size <0 then block free  else block reserved */
 struct freeListNode *forward, *backward;
} Buddy_FreeListNode;


typedef struct {
 int M;						/* log2(sizeof(heap) */
 Buddy_FreeListNode* freeList;			/* a pointer to an array of struct ! \
 						   NOTE the [+1]: freeList must be declared as:  Buddy_FreeListNode freeList[M+1]; */
 int* heap;					/* pointer to the (array of) heap mem space */
} Buddy_Heap;

extern Buddy_Heap heap;
extern Buddy_FreeListNode freeList[];


extern void* Buddy_Alloc(Buddy_Heap* h, size_t size, int zeroFill);
extern int Buddy_Dealloc(Buddy_Heap* h, void* addr);

extern int Buddy_GetFreeMemInHeap(Buddy_Heap* h, int list[/*Log2HeapSize+1*/], int* largest);
/* list[k] contains the number of free blocks with size (2^k) and returns the total of free mem */

extern int Buddy_CheckHeapIntact(Buddy_Heap* h);
/* does plausibility checks on heap's internals, returns TRUE if o.k. */

extern int Buddy_Init(int* heapMem, int log2HeapSize, Buddy_FreeListNode* freeList, Buddy_Heap* h);
/* NOTE: heapMem[0] must start at an addr with lowest M bits =0 !
   NOTE the [+1]: freeList must be declared as:  Buddy_FreeListNode freeList[M+1]; ! */

