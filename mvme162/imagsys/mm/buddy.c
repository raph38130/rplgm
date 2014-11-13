/* $Id$ 

h	buddy.c:
	dynamic mem management with the (exponential) buddy system
	advantages:
	 -) avoids problems of (external) fragmentation
	 -) requires very little (constant) time to merge adjacent empty blocks
	disadvantages:
	 -) blocks come in a limited assortment of size (exponential: 1,2,4,8,...)
	    => certain (internal) fragmnentation
	see Aho, Hopcroft, Ullman: Datastructures and Algorithms, p. 400ff
	see Knuth, Vol. 1, p. 443ff
impl	ADI C
log	14.03.95/Rolf: creation
	20.03.95/Rolf: extended to several heaps
$Log$
*/


#include "mem.h"
#include "register.h"

/*....................................................................*/

#define Max(X,Y) ((X)>(Y)?(X):(Y))

static inline int Roundup(size_t size) {
 /* returns k so that 2^(k-1) < size <= 2^k */
 register int k= 0, f= 1;
 while (size >f) {
  f *= 2;
  k++;
 }
 return k;
}


void* Buddy_Alloc(Buddy_Heap* h, size_t size, int zeroFill) {
 register int j, k;
 Buddy_FreeListNode *L, *R;
 int* p;
 
 if (size +sizeof(Buddy_FreeListNode) > (1<<h->M)) return NULL;
 j= k= Roundup(Max(size +sizeof(L->size), sizeof(Buddy_FreeListNode))); /* find k with  2^(k-1) < size <= 2^k */
 cli();							/* b'cause freeList[] etc are shard vars !!! */
 while ((j <=h->M) && (h->freeList[j].forward == &h->freeList[j])) /* R1: {find free block} */
  j++; 
 if (j <=h->M) {						/* R2: remove block (i.e. first list entry) from freeList */ 
  L= h->freeList[j].forward;
  h->freeList[j].forward= L->forward;
  L->forward->backward= (Buddy_FreeListNode*)&h->freeList[j];
  L->size= k;						/* tag block as reserved */
  while (j != k) {					/* R3: split required ? */
   j--;							/* R4: yes, split ! */
   R= (void*)L +(1<<j);
   R->size= -j;						/* mark block as free with its negative size */
   R->forward= R->backward= (Buddy_FreeListNode*)&h->freeList[j]; /* insert free block with size j in freeList */
   h->freeList[j].forward= h->freeList[j].backward = R;
  }
  //RTK_RestoreIntsEnable;
  if (zeroFill) {					/* initialize block with zeros */
   p= ((int*)L) +sizeof(L->size);
   for (j= 0; j <size; j++)  *p++= 0;
  }
  return ((void*)L) +sizeof(L->size);			/* available block starts at (L+1) */
 }
 else {							/* no available block with size k */
  //RTK_RestoreIntsEnable;
  return 0;						/* signal out of heap space */
 }
}

/*....................................................................*/

static inline void* Buddy(int k, void* a) {
 /* computes buddy's addr of block with size 2^k and start addr a */
 if ((int)a & (1<<k))					/* if (BitTest(a, k)) */
  return a -(1<<k);					/* xxx1xxxx : buddy at left */
 else 
  return a +(1<<k);					/* xxx0xxxx : buddy at right */
}

int Buddy_Dealloc(Buddy_Heap* h, void* addr) {
 register int k;
 Buddy_FreeListNode *L, *R;
 
  cli();						/* b'cause freeList[] etc are shard vars !!! */
  if ((addr <(void*)&h->heap[0]) || (addr >=(void*)&h->heap[1<<h->M]))  return -1; /* addr outside of valid range of heap */
  L= addr -sizeof(L->size);
  k= L->size;						/* get block's size k at L-sizeof() */
  if ((k <0) || (k >h->M))  return -1;			/* block never allocated, already free'd, its size-field was overwritten */

  for (;;) {
   R= Buddy(k, L);					/* S1: (is buddy free ?) */
   if ((k ==h->M) 					/* max heapsize is free */ \
       || (R->size >0) 					/* buddy is reserved/in-use */ \
       || (R->size != -k))				/* buddy is free but not of same size */
    break;						/* goto S3 */
   R->backward->forward= R->forward;			/* S2: (combine with buddy) */
   R->forward->backward= R->backward;
   k++;
   if (R <L)  L= R;					/* L's buddy was on the left */
  }							/* goto S1 */
							/* S3: put block in freeList[k] */
  L->size= -k;						/* mark block as free */
  L->forward= h->freeList[k].forward;			/* and insert it at head of list */
  h->freeList[k].forward->backward= L;
  L->backward= &h->freeList[k];
  h->freeList[k].forward= L;
 //RTK_RestoreIntsEnable;
 return -1;
}

/*....................................................................*/

int Buddy_GetFreeMemInHeap(Buddy_Heap* h, int list[/*Log2HeapSize+1*/], int* largest) {
 int n, k, freeMem= 0;
 Buddy_FreeListNode* p;

  if (!Buddy_CheckHeapIntact(h)) return -1;
  cli();						/* b'cause freeList[] etc are shard vars !!! */
  for (k= 0; k <=h->M; k++) {
   n= 0;
   p= h->freeList[k].forward;
   while (p != &h->freeList[k]) {
    n++;
    p= p->forward;
   }
   list[k]= n;
   freeMem += n*(1<<k);
   if (n !=0) *largest= (1<<k);
  }
 //RTK_RestoreIntsEnable;
 return freeMem;
}


int Buddy_CheckHeapIntact(Buddy_Heap* h) {
 int k, nForward, nBackward, nMax;
 Buddy_FreeListNode* p;

   cli();						/* b'cause freeList[] etc are shard vars !!! */
   for (k= 0; k <=h->M; k++) {
   if (h->freeList[k].size != -k)  return FALSE;	/* check size in freeList (which should never change after Buddy_Init()) */
   nForward= nBackward= 0;
   nMax= (1<<h->M)/(1<<k);
   p= h->freeList[k].forward;
   while (p != &h->freeList[k]) {			/* go through forward pointer list */
    if (++nForward >nMax)  return FALSE;		/* too many blocks of size 2^k, due to wrong pointer etc. */
    if (p->size !=-k)  return FALSE;			/* check size in blocks */
    p= p->forward;
   }
   p= h->freeList[k].backward;
   while (p != &h->freeList[k]) {			/* go through backward pointer list */
    if (++nBackward >nMax)  return FALSE;
    p= p->backward;
   }
   if (nForward != nBackward)  return FALSE;		/* not the same number of elements forwards and backwards ? */
  } 
 //RTK_RestoreIntsEnable;
 return TRUE;
}


/*....................................................................*/

int Buddy_Init(int* heapMem, int log2HeapSize, Buddy_FreeListNode* freeList, Buddy_Heap* h) {
 int i;
 Buddy_FreeListNode* nodePtr;
 
 if (((int)heapMem & ((1<<log2HeapSize)-1)) !=0x00)  return -1; 	/* heapMem[0] must start at an addr with lowest M bits =0 ! */
 h->M= log2HeapSize;
 h->freeList= freeList;
 h->heap= heapMem;
 for (i= 0; i< h->M; i++) {
  h->freeList[i].size= -i;					/* optional, for debugging only */
  h->freeList[i].forward= h->freeList[i].backward= (Buddy_FreeListNode*)&h->freeList[i];
 }
 h->freeList[h->M].size= -h->M;
 h->freeList[h->M].forward= h->freeList[h->M].backward= (Buddy_FreeListNode*)&h->heap[0];
 nodePtr= (Buddy_FreeListNode*)&h->heap[0];
 nodePtr->size= -h->M;
 nodePtr->forward= nodePtr->backward= (Buddy_FreeListNode*)&h->freeList[h->M];
 return 0;
}

