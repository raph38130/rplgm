/*
 * allocation memoire
 * 24 / 12 / 97 allocation memoire simplisime
 *              prévoir vm
 * 10 / 04 / 98 buddy system
 * $Id$
 */

#include "mem.h"

//en dessous de cette limite (incluse), utilisé par e2s
#define LAST_E2S_PAGE	0x14000


Buddy_Heap heap;
Buddy_FreeListNode freeList[log2heapsize+1];

int t_bitmap_pages_allouees_DRAM[TAILLE_BITMAP_PAGES_ALLOUEES_DRAM];
int t_bitmap_pages_allouees_SRAM[TAILLE_BITMAP_PAGES_ALLOUEES_SRAM];



/******************************************************************************/
/*                                                                            */
/*                                AllocPhysPage                               */
/*                                                                            */
/*   But:                                                                     */
/*      Alloue une page (4k.) de memoire en DRAM ou SRAM                                     */
/*                                                                            */
/******************************************************************************/
char * PageAlloc(int type)
{
int i,j, trouve,
    TAILLE_BITMAP_PAGES_ALLOUEES,
    *t_bitmap_pages_allouees;

switch (type) {
   case DRAM : 	
   		TAILLE_BITMAP_PAGES_ALLOUEES=TAILLE_BITMAP_PAGES_ALLOUEES_DRAM;
    		t_bitmap_pages_allouees=t_bitmap_pages_allouees_DRAM;
		break;
   case SRAM : 	
   		TAILLE_BITMAP_PAGES_ALLOUEES=TAILLE_BITMAP_PAGES_ALLOUEES_SRAM;
    		t_bitmap_pages_allouees=t_bitmap_pages_allouees_SRAM;
   		break;
   default : return NULL;
   }  
   
trouve = 0;
for (i = 0; i < TAILLE_BITMAP_PAGES_ALLOUEES; i++)
   {
   for ( j = 0; j < 32; j++)
      if ((t_bitmap_pages_allouees[i] & ( 1 << j)) == 0)
	 { trouve = 1; break; }
   if (trouve) break;
   }
if (! trouve) return((char *)0);
t_bitmap_pages_allouees[i] = t_bitmap_pages_allouees[i] | ( 1 << j);
return (type==SRAM) ? ((char *) ((i * 32 + j) * 4096+SRAM_BASE))
		    : ((char *) ((i * 32 + j) * 4096));
}

/******************************************************************************/
/*                                                                            */
/*                                FreePhysPage                                */
/*                                                                            */
/*   But:                                                                     */
/*      Libere une page de memoire                                            */
/*                                                                            */
/******************************************************************************/
void PageFree(char * adr, int type)
{
unsigned int apage;
int nopage, i, j,
    TAILLE_BITMAP_PAGES_ALLOUEES,
    *t_bitmap_pages_allouees;

switch (type) {
   case DRAM : 	TAILLE_BITMAP_PAGES_ALLOUEES=TAILLE_BITMAP_PAGES_ALLOUEES_DRAM;
    		t_bitmap_pages_allouees=t_bitmap_pages_allouees_DRAM;
		break;
   case SRAM : 	TAILLE_BITMAP_PAGES_ALLOUEES=TAILLE_BITMAP_PAGES_ALLOUEES_SRAM;
    		t_bitmap_pages_allouees=t_bitmap_pages_allouees_SRAM;
		adr -= SRAM_BASE;
   		break;
   default : return ;
   }

apage = (unsigned int) adr & 0xfffff000;
nopage = apage / 4096;
i = nopage / 32;
j = nopage % 32;
t_bitmap_pages_allouees[i] = t_bitmap_pages_allouees [i] & ~( 1 << j);
}

void init_allocation_par_page()
{
int i, j, nopage;
paddr_t lastpage, apage;
char s[20];

/*   initialisation des tableau t_bitmap_pages_allouees   */
/*   -------------------------------------------------   */
//   on sort e2s en DRAM

//print("init dram");
for (i = 0; i < TAILLE_BITMAP_PAGES_ALLOUEES_DRAM; i++)
   t_bitmap_pages_allouees_DRAM[i] = 0;
lastpage = (paddr_t) LAST_E2S_PAGE;

for (apage = 0; apage <= lastpage; apage = apage + 4096)
   {
   nopage = apage / 4096;
   i = nopage / 32;
   j = nopage % 32;
   t_bitmap_pages_allouees_DRAM[i] = t_bitmap_pages_allouees_DRAM[i] | ( 1 << j);
   }
//SRAM libre en totalité (162bug utilise 8k dans cette zone lors d'un reset)    
//print("init sram");
for (i = 0; i < TAILLE_BITMAP_PAGES_ALLOUEES_SRAM; i++)
   t_bitmap_pages_allouees_SRAM[i] = 0;


}


void init_buddy()
{
Buddy_Init((int*)BASE,log2heapsize, freeList, &heap);
}


void init_mm()
{
init_allocation_par_page();
}
